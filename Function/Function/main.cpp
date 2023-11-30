#include <iostream>
#include <memory>
#include <functional>

template <typename Signature>
class Function;

template <typename ReturnType, typename... Args>
class Function <ReturnType(Args...)>
{
private:
	class BaseFunctionImplement
	{
	public:
		BaseFunctionImplement() {};
		virtual ~BaseFunctionImplement() {};
		virtual ReturnType CallingMethod(Args...) = 0;
	};
	template <typename TempFunc>
	class FunctionImplement : public BaseFunctionImplement
	{
	private:
		TempFunc tempFunc;

	public:
		FunctionImplement(TempFunc func) :tempFunc(func) {};

		ReturnType CallingMethod(Args... args) override
		{
			return tempFunc(args...);
		}
	};
public:
	using FunctionPointer = std::unique_ptr<BaseFunctionImplement>;
	FunctionPointer functionPointer;

	Function() :functionPointer(nullptr) {};

	template <typename TFunc>
	Function(TFunc func) : functionPointer(new FunctionImplement<TFunc>(func)) {};


	ReturnType operator()(Args... args)
	{
		return functionPointer->CallingMethod(args...);
	}
};

double Multi(double x, double y)
{
	return x * y;
}

double Summ(double x, double y)
{
	return x + y;
}


void print_num(int i)
{
	std::cout << i << '\n';
}

struct PrintNum
{
	void operator()(int i) const
	{
		std::cout << i << '\n';
	}
};

int main() {

	Function<double(double, double)> CustomFunctionHolder;

	CustomFunctionHolder = Multi;
	std::cout << "Result " << CustomFunctionHolder(24.4, 4) << "\n";

	CustomFunctionHolder = Summ;
	std::cout << "Result " << CustomFunctionHolder(11, 12.1) << "\n";



	// store a free function
	Function<void(int)> f_display = print_num;
	f_display(-9);

	// store a lambda
	Function<void()> f_display_42 = []() { print_num(42); };
	f_display_42();

	// store the result of a call to std::bind
	Function<void()> f_display_31337 = std::bind(print_num, 31337);
	f_display_31337();

	// store a call to a function object
	Function<void(int)> f_display_obj = PrintNum();
	f_display_obj(18);

	auto factorial = [](int n)
	{
		// store a lambda object to emulate "recursive lambda"; aware of extra overhead
		Function<int(int)> fac = [&](int n) { return (n < 2) ? 1 : n * fac(n - 1); };
		// note that "auto fac = [&](int n) {...};" does not work in recursive calls
		return fac(n);
	};
	for (int i{ 5 }; i != 8; ++i)
		std::cout << i << "! = " << factorial(i) << ";  ";
	std::cout << '\n';


	std::cout << "Press Enter to exit...";
	std::cin.get();  // Wait for the user to press Enter


	return 0;
}