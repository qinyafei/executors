#include <experimental/executor>
#include <experimental/future>
#include <cassert>
#include <stdexcept>

int function_count = 0;
int handler_count = 0;

int function1()
{
  return ++function_count;
}

struct function2
{
  function2() {}
  int operator()() { return ++function_count; }
};

struct function3
{
  function3() {}
  function3(const function3&) = delete;
  function3(function3&&) {}
  int operator()() { return ++function_count; }
};

int function_throw()
{
  throw std::runtime_error("oops");
}

void handler1(int)
{
  ++handler_count;
}

struct handler2
{
  handler2() {}
  void operator()(int) { ++handler_count; }
};

struct handler3
{
  handler3() {}
  handler3(const handler3&) = delete;
  handler3(handler3&&) {}
  void operator()(int) && { ++handler_count; }
};

int main()
{
  function2 f2;
  const function2 cf2;
  function3 f3;

  handler2 h2;
  const handler2 ch2;
  handler3 h3;

  std::experimental::dispatch(function1);
  std::experimental::dispatch(function1, handler1);
  std::experimental::dispatch(function1, &handler1);
  std::experimental::dispatch(function1, handler2());
  std::experimental::dispatch(function1, h2);
  std::experimental::dispatch(function1, ch2);
  std::experimental::dispatch(function1, handler3());
  std::experimental::dispatch(function1, std::move(h3));
  std::future<int> fut1 = std::experimental::dispatch(function1, std::experimental::use_future);
  fut1.get();

  std::experimental::dispatch(&function1);
  std::experimental::dispatch(&function1, handler1);
  std::experimental::dispatch(&function1, &handler1);
  std::experimental::dispatch(&function1, handler2());
  std::experimental::dispatch(&function1, h2);
  std::experimental::dispatch(&function1, ch2);
  std::experimental::dispatch(&function1, handler3());
  std::experimental::dispatch(&function1, std::move(h3));
  std::future<int> fut2 = std::experimental::dispatch(&function1, std::experimental::use_future);
  fut2.get();

  std::experimental::dispatch(function2());
  std::experimental::dispatch(function2(), handler1);
  std::experimental::dispatch(function2(), &handler1);
  std::experimental::dispatch(function2(), handler2());
  std::experimental::dispatch(function2(), h2);
  std::experimental::dispatch(function2(), ch2);
  std::experimental::dispatch(function2(), handler3());
  std::experimental::dispatch(function2(), std::move(h3));
  std::future<int> fut3 = std::experimental::dispatch(function2(), std::experimental::use_future);
  fut3.get();

  std::experimental::dispatch(f2);
  std::experimental::dispatch(f2, handler1);
  std::experimental::dispatch(f2, &handler1);
  std::experimental::dispatch(f2, handler2());
  std::experimental::dispatch(f2, h2);
  std::experimental::dispatch(f2, ch2);
  std::experimental::dispatch(f2, handler3());
  std::experimental::dispatch(f2, std::move(h3));
  std::future<int> fut4 = std::experimental::dispatch(f2, std::experimental::use_future);
  fut4.get();

  std::experimental::dispatch(cf2);
  std::experimental::dispatch(cf2, handler1);
  std::experimental::dispatch(cf2, &handler1);
  std::experimental::dispatch(cf2, handler2());
  std::experimental::dispatch(cf2, h2);
  std::experimental::dispatch(cf2, ch2);
  std::experimental::dispatch(cf2, handler3());
  std::experimental::dispatch(cf2, std::move(h3));
  std::future<int> fut5 = std::experimental::dispatch(cf2, std::experimental::use_future);
  fut5.get();

  std::experimental::dispatch(function3());
  std::experimental::dispatch(function3(), handler1);
  std::experimental::dispatch(function3(), &handler1);
  std::experimental::dispatch(function3(), handler2());
  std::experimental::dispatch(function3(), h2);
  std::experimental::dispatch(function3(), ch2);
  std::experimental::dispatch(function3(), handler3());
  std::experimental::dispatch(function3(), std::move(h3));
  std::future<int> fut6 = std::experimental::dispatch(function3(), std::experimental::use_future);
  fut6.get();

  std::experimental::dispatch(std::move(f3));
  std::experimental::dispatch(std::move(f3), handler1);
  std::experimental::dispatch(std::move(f3), &handler1);
  std::experimental::dispatch(std::move(f3), handler2());
  std::experimental::dispatch(std::move(f3), h2);
  std::experimental::dispatch(std::move(f3), ch2);
  std::experimental::dispatch(std::move(f3), handler3());
  std::experimental::dispatch(std::move(f3), std::move(h3));
  std::future<int> fut7 = std::experimental::dispatch(std::move(f3), std::experimental::use_future);
  fut7.get();

  assert(function_count == 63);
  assert(handler_count == 49);

  std::future<int> fut8 = std::experimental::dispatch(function_throw, std::experimental::use_future);
  try
  {
    fut8.get();
    assert(0);
  }
  catch (std::exception& e)
  {
    assert(e.what() == std::string("oops"));
  }

  std::future<int> fut9 = std::experimental::dispatch(std::experimental::package(function_throw));
  try
  {
    fut9.get();
    assert(0);
  }
  catch (std::exception& e)
  {
    assert(e.what() == std::string("oops"));
  }
}
