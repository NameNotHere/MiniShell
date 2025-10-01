#!/bin/bash

echo "=== Final heredoc delimiter test ==="
echo "USER=$USER"

echo
echo "1. Double-quoted delimiter (should terminate with literal \$USER):"
./minishell << 'EOF'
cat << "$USER"
test content
$USER
exit
EOF

echo
echo "2. Single-quoted delimiter (should terminate with literal \$USER):"
./minishell << 'EOF'
cat << '$USER'
test content  
$USER
exit
EOF

echo
echo "3. Unquoted delimiter (should terminate with expanded value 'orhan'):"
./minishell << 'EOF'
cat << $USER
test content
orhan
exit
EOF

echo
echo "4. Simple literal delimiter (should terminate with 'test'):"
./minishell << 'EOF'
cat << test
test content
test
exit
EOF

echo
echo "=== All heredoc tests completed successfully! ==="
