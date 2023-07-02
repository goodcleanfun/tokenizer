#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "scanner.h"
#include "python_strings.h"

static PyObject *tokens(PyObject *self, PyObject *args) {
    char *text;
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    return Py_BuildValue("s", tokenize(text));
}


static PyObject *py_tokens(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *text;
    uint32_t whitespace = 0;
    static char* kwlist[] = {"text", "whitespace", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|I:tokens", kwlist, &text, &whitespace)) {
        return NULL;
    }

    token_array *tokens;

    if (!whitespace) {
        tokens = tokenize(text);
    } else {
        tokens = tokenize_keep_whitespace(text);
    }
    if (tokens == NULL) {
        goto error_free_input;
    }

    size_t num_tokens = tokens->n;
    PyObject *result = PyTuple_New(num_tokens);
    if (!result) {
        goto error_free_tokens;
    }

    PyObject *tuple;

    token_t token;
    for (size_t i = 0; i < num_tokens; i++) {
        token = tokens->a[i];
        tuple = Py_BuildValue("III", token.offset, token.len, token.type);
        if (PyTuple_SetItem(result, i, tuple) < 0) {
            goto error_free_tokens;
        }
    }

    free(text);
    free(tokens);

    return result;

error_free_tokens:
    free(tokens);
error_free_input:
    free(text);
    return 0;
}



static PyMethodDef tokenize_methods[] = {
    {"tokens", (PyCFunction)py_tokens, METH_VARARGS | METH_KEYWORDS, "tokens(text, whitespace=False)"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static struct PyModuleDef tokenize_module = {
    PyModuleDef_HEAD_INIT,
    "_scanner",
    NULL,
    -1,
    tokenize_methods
};

PyMODINIT_FUNC PyInit_tokenize(void) {
    return PyModule_Create(&tokenize_module);
}

