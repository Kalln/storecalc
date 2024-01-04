all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java
	javac -d classes -sourcepath src src/org/ioopm/calculator/Test.java

inputoutputtest: all
	java -cp classes org.ioopm.calculator.Calculator < tests/input.txt > tests/output.txt
	diff tests/output.txt tests/expected_output.txt
	java -cp classes org.ioopm.calculator.Calculator < tests/input_2.txt > tests/output_2.txt
	diff tests/output_2.txt tests/expected_output_2.txt


run: all
	java -cp classes org.ioopm.calculator.Calculator

runtest: all
	java -cp classes org.ioopm.calculator.Test

test: inputoutputtest runtest

clean:
	rm -rf classes
