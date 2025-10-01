No 42 HEADER support in markdown, so I am just adding our user names:
tda-roch <tda-roch@student.codam.nl>
&
otanovic <otanovic@student.codam.nl>

TODO: remove this folder before submitting project.

Testers folder contains code EXTERNAL from the minishell project. These files
should NOT be included in the project evaluation.
They are openly available testers created by the community for the sole purpose
of testing minishell project features. for this reason we are including them so
we (tda-roch & otanovic) can use the same version of tests.

the tests are all from a specific version of tests from authors githubs,
cloned circa Sept 2025/ Oct 2025, with the following modifications:

- .git folders were removed to avoid conflicts with minishell git handling.
- Some changes in the original test files may have been added to conform with
the current location and our minishell project characteristics. For instance,
removal of tests of unrequired features, or executable paths (most tests assume
they are installed either at the minishell folder or one folder above).
- Some tests or test versions may be added, or feedbacks from existing tests
changed for some reason.

## Custom Heredoc Tests

We've added several custom test scripts for heredoc delimiter functionality:

- **test_heredoc.sh**: Basic heredoc tests comparing bash and minishell behavior
- **test_heredoc_fix.sh**: Tests for the heredoc delimiter fix implementation  
- **test_heredoc_complete.sh**: Comprehensive heredoc termination tests
- **test_final_heredoc.sh**: Final verification of all heredoc delimiter cases

These tests verify the correct handling of quoted vs unquoted heredoc delimiters:
- `cat << "$USER"` → terminate with literal `$USER`
- `cat << '$USER'` → terminate with literal `$USER` 
- `cat << $USER` → terminate with expanded value `orhan`

The tests ensure minishell behavior matches bash exactly for heredoc delimiter expansion.