#include <experimental/executor>
#include <experimental/future>
#include <experimental/loop_scheduler>
#include <atomic>
#include <cassert>

std::atomic<int> function_count(0);
std::atomic<int> handler_count(0);

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
  void operator()(int) { ++handler_count; }
};

int main()
{
  function2 f2;
  const function2 cf2;
  function3 f3;

  handler2 h2;
  const handler2 ch2;
  handler3 h3;

  std::experimental::loop_scheduler scheduler;
  std::experimental::executor ex = get_executor(scheduler);

  ex = std::experimental::system_executor();
  assert(&ex.context() == &std::experimental::system_executor().context());

  std::experimental::post(function1, ex.wrap(handler1));
  std::experimental::post(function1, ex.wrap(&handler1));
  std::experimental::post(function1, ex.wrap(handler2()));
  std::experimental::post(function1, ex.wrap(h2));
  std::experimental::post(function1, ex.wrap(ch2));
  std::experimental::post(function1, ex.wrap(handler3()));
  std::experimental::post(function1, ex.wrap(std::move(h3)));
  std::future<int> fut1 = std::experimental::post(function1, ex.wrap(std::experimental::use_future));
  fut1.get();

  std::experimental::post(&function1, ex.wrap(handler1));
  std::experimental::post(&function1, ex.wrap(&handler1));
  std::experimental::post(&function1, ex.wrap(handler2()));
  std::experimental::post(&function1, ex.wrap(h2));
  std::experimental::post(&function1, ex.wrap(ch2));
  std::experimental::post(&function1, ex.wrap(handler3()));
  std::experimental::post(&function1, ex.wrap(std::move(h3)));
  std::future<int> fut2 = std::experimental::post(&function1, ex.wrap(std::experimental::use_future));
  fut2.get();

  std::experimental::post(function2(), ex.wrap(handler1));
  std::experimental::post(function2(), ex.wrap(&handler1));
  std::experimental::post(function2(), ex.wrap(handler2()));
  std::experimental::post(function2(), ex.wrap(h2));
  std::experimental::post(function2(), ex.wrap(ch2));
  std::experimental::post(function2(), ex.wrap(handler3()));
  std::experimental::post(function2(), ex.wrap(std::move(h3)));
  std::future<int> fut3 = std::experimental::post(function2(), ex.wrap(std::experimental::use_future));
  fut3.get();

  std::experimental::post(f2, ex.wrap(handler1));
  std::experimental::post(f2, ex.wrap(&handler1));
  std::experimental::post(f2, ex.wrap(handler2()));
  std::experimental::post(f2, ex.wrap(h2));
  std::experimental::post(f2, ex.wrap(ch2));
  std::experimental::post(f2, ex.wrap(handler3()));
  std::experimental::post(f2, ex.wrap(std::move(h3)));
  std::future<int> fut4 = std::experimental::post(f2, ex.wrap(std::experimental::use_future));
  fut4.get();

  std::experimental::post(cf2, ex.wrap(handler1));
  std::experimental::post(cf2, ex.wrap(&handler1));
  std::experimental::post(cf2, ex.wrap(handler2()));
  std::experimental::post(cf2, ex.wrap(h2));
  std::experimental::post(cf2, ex.wrap(ch2));
  std::experimental::post(cf2, ex.wrap(handler3()));
  std::experimental::post(cf2, ex.wrap(std::move(h3)));
  std::future<int> fut5 = std::experimental::post(cf2, ex.wrap(std::experimental::use_future));
  fut5.get();

  std::experimental::post(function3(), ex.wrap(handler1));
  std::experimental::post(function3(), ex.wrap(&handler1));
  std::experimental::post(function3(), ex.wrap(handler2()));
  std::experimental::post(function3(), ex.wrap(h2));
  std::experimental::post(function3(), ex.wrap(ch2));
  std::experimental::post(function3(), ex.wrap(handler3()));
  std::experimental::post(function3(), ex.wrap(std::move(h3)));
  std::future<int> fut6 = std::experimental::post(function3(), ex.wrap(std::experimental::use_future));
  fut6.get();

  std::experimental::post(std::move(f3), ex.wrap(handler1));
  std::experimental::post(std::move(f3), ex.wrap(&handler1));
  std::experimental::post(std::move(f3), ex.wrap(handler2()));
  std::experimental::post(std::move(f3), ex.wrap(h2));
  std::experimental::post(std::move(f3), ex.wrap(ch2));
  std::experimental::post(std::move(f3), ex.wrap(handler3()));
  std::experimental::post(std::move(f3), ex.wrap(std::move(h3)));
  std::future<int> fut7 = std::experimental::post(std::move(f3), ex.wrap(std::experimental::use_future));
  fut7.get();

  std::this_thread::sleep_for(std::chrono::seconds(1));
  assert(function_count == 56);
  assert(handler_count == 49);
}