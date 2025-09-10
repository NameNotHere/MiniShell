#!/bin/bash

# Test simple echo
echo "Testing simple echo:"
echo "echo hello" | timeout 5 ./minishell
echo ""

# Test echo with pipe
echo "Testing echo with pipe:"
echo "echo hello | echo world" | timeout 5 ./minishell
echo ""
