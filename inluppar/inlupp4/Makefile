all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

test:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Test.java

run:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java
	java -cp classes org.ioopm.calculator.Calculator

runtest:
	java -cp classes org.ioopm.calculator.Test

clean:
	rm -rf classes
