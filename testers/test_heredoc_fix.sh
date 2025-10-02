#!/bin/bash

echo "=== Testing minishell heredoc fix ==="
echo "USER=$USER"

echo
echo "1. Testing: cat << \"\$USER\" (quoted delimiter - should terminate with '\$USER')"
./minishell << 'EOF'
cat << "$USER"
why
not
$USER
exit
EOF

echo
echo "2. Testing: cat << '\$USER' (single-quoted delimiter - should terminate with '\$USER')"
./minishell << 'EOF'
cat << '$USER'
why
not
$USER
exit
EOF

echo
echo "3. Testing: cat << \$USER (unquoted delimiter - should terminate with 'orhan')"
./minishell << 'EOF'
cat << $USER
why
not
orhan
exit
EOF

echo
echo "=== All tests completed ==="
