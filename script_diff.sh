# note: ls and ft_ls update all of your printed entries access date !
# this may give confusing diffs with -u option !

make
./ft_ls $* > /tmp/my_ls_stdout 2> /dev/null
./ft_ls $* > /dev/null 2> /tmp/my_ls_stderr
ls $* > /tmp/true_ls_stdout 2> /dev/null
ls $* > /dev/null 2> /tmp/true_ls_stderr 
diff /tmp/true_ls_stdout /tmp/my_ls_stdout > /tmp/diff_ls_stdout
let "res_stdout = $?"
diff /tmp/true_ls_stderr /tmp/my_ls_stderr > /tmp/diff_ls_stderr
let "res_stderr = $?"
if [ $res_stdout = 1 ] && [ $res_stderr = 1 ]
then
	echo "(stdout): test with args: "$* >> "/tmp/diff_ls_stdout"
	echo "(stderr): test with args: "$* >> "/tmp/diff_ls_stderr"
	cat /tmp/diff_ls_stdout > diff_ls_trace
	cat /tmp/diff_ls_stderr >> diff_ls_trace
	vim diff_ls_trace
elif [ $res_stdout = 1 ]
then
	echo "(stdout): test with args: "$* >> "/tmp/diff_ls_stdout"
	cat /tmp/diff_ls_stdout > diff_ls_trace
	echo "stderr is ok :)\n" >> diff_ls_trace
	vim diff_ls_trace
elif [ $res_stderr = 1 ]
then
	echo "(stderr): test with args: "$* >> "/tmp/diff_ls_stderr"
	echo "stdout is ok :)\n" > diff_ls_trace
	cat /tmp/diff_ls_stderr >> diff_ls_trace
	vim diff_ls_trace
else
	echo "\x1B[32mOK\033[0m :)"
fi
