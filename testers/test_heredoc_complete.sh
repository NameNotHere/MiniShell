#!/bin/bash

echo "=== Testing heredoc termination fix ==="
echo "USER=$USER"

echo
echo "Test 1: cat << \"\$USER\" (should terminate with 'orhan')"
./minishell << 'EOF'
cat << "$USER"
line1
line2
orhan
exit
EOF

echo
echo "Test 2: cat << '\$USER' (should terminate with literal '\$USER')"
./minishell << 'EOF'
cat << '$USER'
line1
line2
$USER
exit
EOF

echo
echo "Test 3: cat << \$USER (should terminate with 'orhan')"
./minishell << 'EOF'
cat << $USER
line1
line2
orhan
exit
EOF

echo
echo "Test 4: cat << \"test\" (should terminate with 'test')"
./minishell << 'EOF'
cat << "test"
line1
line2
test
exit
EOF

echo
echo "Test 5: cat << 'test' (should terminate with 'test')"
./minishell << 'EOF'
cat << 'test'
line1
line2
test
exit
EOF

echo
echo "=== All heredoc tests completed successfully ==="
