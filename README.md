Minishell

run this command to pipe results to file:
mstest m | sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" > test.txt

ot this for jsut the errors 
mstest m | sed -r "s/\x1B\[[0-9;]*[a-zA-Z]//g" | grep -E "❌|SIGSEGV|SIGABRT|#"


slowing down machine:
for n in $(nproc); do yes >/dev/null &; done
➜  minishell git:(main) ✗ nproc
20
➜  minishell git:(main) ✗ for n in $(nproc); do yes >/dev/null &; done
[3] 1721430
➜  minishell git:(main) ✗ for n in $(nproc); do yes >/dev/null &; done

after that
htop

after that killall yes