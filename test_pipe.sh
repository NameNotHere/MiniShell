#!/bin/bash

echo "Testing minishell pipe functionality..."

# Test 1: Simple pipe with built-ins
echo "Test 1: echo hello | echo world"
echo "echo hello | echo world" | timeout 5 ./minishell
echo "Exit code: $?"
echo

# Test 2: Built-in to external command
echo "Test 2: echo hello | cat"
echo "echo hello | cat" | timeout 5 ./minishell
echo "Exit code: $?"
echo

# Test 3: External to built-in  
echo "Test 3: /bin/echo hello | echo world"
echo "/bin/echo hello | echo world" | timeout 5 ./minishell
echo "Exit code: $?"
echo

echo "Testing completed."
