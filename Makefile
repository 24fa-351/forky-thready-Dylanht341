forky: forky.c 
	gcc -o forky forky.c

results: forky
	(echo "Pattern 1:" 1>&2; ./forky 1 ${shell bc --expression="$$RANDOM % 15 + 5"}; \
	echo 1>&2; \
	echo "Pattern 2:" 1>&2; ./forky 2 ${shell bc --expression="$$RANDOM % 15 + 5"}; \
	echo 1>&2; \
	echo "Pattern 3:" 1>&2; ./forky 3 ${shell bc --expression="$$RANDOM % 20 + 5"}) \
		2> results.txt

clean:
	rm -f forky



