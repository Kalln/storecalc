all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

test: all
	java -cp classes org.ioopm.calculator.Calculator < tests/input.txt > tests/output.txt
	diff tests/output.txt tests/expected_output.txt


run: all
	java -cp classes org.ioopm.calculator.Calculator

runtest:
	java -cp classes org.ioopm.calculator.Test

clean:
	rm -rf classes
