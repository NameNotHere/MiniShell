#!/bin/bash

echo "=== Testing heredoc with quoted delimiter in bash ==="
echo "USER=$USER"

echo "1. Testing: cat << \"\$USER\""
cat << "$USER"
why
not
orhan

echo "2. Testing: cat << \$USER"  
cat << $USER
why
not
orhan

echo "3. Testing: cat << '\$USER'"
cat << '$USER'
why
not
$USER
