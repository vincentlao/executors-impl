#include <experimental/thread_pool>
#include <iostream>

namespace execution = std::experimental::execution;
using std::experimental::static_thread_pool;

using executor = execution::executor<
  execution::oneway_t,
  execution::single_t,
  execution::blocking_t::possibly_t,
  execution::blocking_t::never_t>;

int main()
{
  static_thread_pool pool{1};
  executor ex = execution::require(pool.executor(), execution::blocking.possibly);
  std::cout << "before submission\n";
  ex.execute([ex = execution::require(ex, execution::blocking.never)]{
      std::cout << "outer starts\n";
      ex.execute([]{ std::cout << "inner\n"; });
      std::cout << "outer ends\n";
    });
  std::cout << "after submission, before wait\n";
  pool.wait();
  std::cout << "after wait\n";
}
