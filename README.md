Minishell

run this command to pipe results to file:
mstest m | sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" > test.txt

ot this for jsut the errors 
mstest m | sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" | grep -E "❌|SIGSEGV|SIGABRT|#"
