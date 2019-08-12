# Earley Parser

***Compile Command: make***

***Run Command: make run***

test.gr is a test grammar file for showing a demo and a sample sentence is used in the main file. Feel free to play with grammar and sentence.

It is one of the well known parsers in ***Natural Language Processing*** in spite of cubic time complexity. It works well with most grammars and smart pruning and implementation strategies can improve its performance by significant factor.

I started with Python implementation which was pretty quick to code but too slow even with pruning, rendering it useless for long sentences (>15 words). Having done the coding in one language, it is not too time consuming to implement the same logic in another programming language. Since time performance is what I am looking for here, I chose ***C***.

As an initial commit, time wise performance is significantly better.

At this stage, performance is improved using pruning strategies:
***left corner filtering, better indexing, filtering grammar of terminals that are not needed, restriction in adding duplicate entries by comparing probabilities, pruning the rules that have lower probabilites***
