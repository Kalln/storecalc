all:
	javac -d classes -sourcepath org org/ioopm/calculator/*.java
	javac -d classes -sourcepath org org/ioopm/calculator/ast/*.java org/ioopm/calculator/ast/atom/*.java
	javac -d classes -sourcepath org org/ioopm/calculator/ast/*.java org/ioopm/calculator/ast/command/*.java org/ioopm/calculator/ast/atom/*.java
	javac -d classes -sourcepath org org/ioopm/calculator/ast/*.java org/ioopm/calculator/ast/unary/*.java org/ioopm/calculator/ast/atom/*.java
	javac -d classes -sourcepath org org/ioopm/calculator/ast/*.java org/ioopm/calculator/ast/binary/*.java org/ioopm/calculator/ast/atom/*.java

run:
	java -cp classes org.ioopm.calculator.Calculator

clean:
	rm -rf classes