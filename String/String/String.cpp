#include <iostream>

class String {
public:
	String(const char* s) : text(s) {	}
	String(const String &s) {
		text = s.text;
	}

	friend std::ostream& operator<<(std::ostream& os, const String& s);
	String operator+(const String& s) const {
		int l1 = length();
		int l2 = l1 + s.length();
		char *t = new char[l2];
		int i = 0;
		for (i; i < l1; i++)
		{
			t[i] = text[i];
		}
		for (i; i < l2; i++) {
			t[i] = s.text[i - l1];
		}
		t[i] = 0;

		return String(t);
	}
	bool operator==(const char* t) const {
		return text == t;
	}


	void clear() {
		text = "";
	}

	inline int length() const {
		return strlen(text);
	}

private:
	const char* text;
};
