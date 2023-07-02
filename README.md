tokenizer
=========

Method for writing super-fast tokenizers/lexers using re2c. A set of regex rules and their associated token types is reduced to a fast, optimized, C-based finite state automaton (FSA).

The rules could be derived from data or written out. Unicode categories are provided and kept up-to-date.

Since there's no one-size-fits-all for tokenizers, this repo is a template using [copier](https://github.com/copier-org/copier) to start a new repo for each tokenizer. The result will be in C using [clib](https://github.com/clibs/clib) with an optional Python binding.
