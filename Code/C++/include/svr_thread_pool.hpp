#pragma once
namespace   svr {
    class ThreadPool {
    public:
        // Members
        boost::asio::thread_pool        thread_pool;        // the pool
        std::vector<std::future<void>>  future_vector;      // futures to wait on

        // Special-Members
        ThreadPool(std::size_t num_threads)
            : thread_pool(num_threads) {}
        ThreadPool(const ThreadPool& other)         = delete;
        ThreadPool& operator=(ThreadPool& other)    = delete;

        // Member-functions
        void                        converge();
        template <typename F> void  push_back(F&&  func);
        void                        shutdown();

    private:
        template<typename F>
        std::future<void> _wrap_task(F&& func) {
            std::promise<void> p;
            auto f = p.get_future();

            boost::asio::post(thread_pool,
                [func = std::forward<F>(func), p = std::move(p)]() mutable {
                    func();
                    p.set_value();
                });

            return f;
        }
    };

    /*==========================================================================
    Member-Function: Add new task to the pool
    --------------------------------------------------------------------------*/ 
    template    <typename F>
    void ThreadPool::push_back(F&&  func)
    {
        future_vector.push_back(_wrap_task(std::forward<F>(func)));
    }
    /*==========================================================================
    Member-Function: waiting until all the assigned work is done
    --------------------------------------------------------------------------*/ 
    void ThreadPool::converge() 
    {
        for (auto &fut : future_vector) fut.get();
        future_vector.clear();
    }
    /*==========================================================================
    Member-Function: Shutting things down
    --------------------------------------------------------------------------*/ 
    void ThreadPool::shutdown()
    {
        thread_pool.join();
    }

}
