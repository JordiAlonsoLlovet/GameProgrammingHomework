#include "String.cpp"


std::ostream& operator<<(std::ostream& os, const String& s) {
	os << s.text;
	return os;
}

int main() {
	String a = String("hellohellohello");
	String b = String("bye");
	a = a + b;
	//a.clear();
	if (b == "bye") {
		std::cout << a.length() << " : " << a;
	}
	

	return 0;
}