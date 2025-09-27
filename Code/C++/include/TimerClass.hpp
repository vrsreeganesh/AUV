namespace   svr {
    struct Timer
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> startpoint;
        std::chrono::time_point<std::chrono::high_resolution_clock> endpoint;
        std::chrono::duration<long long, std::nano>                 duration;
        std::string                                                 filename;
        std::string                                                 functionname;
        
        // constructor
        Timer()         {start();}
        // Timer(std::string logfile_arg): filename(std::move(logfile_arg)) {start();}
        Timer(std::string logfile_arg, std::string func_arg): filename(std::move(logfile_arg)),
                                    functionname(std::move(func_arg))  {start();}
        Timer(std::string func_arg){
            functionname    = func_arg;
            start();
        }

        void start()    {startpoint = std::chrono::high_resolution_clock::now();}
        void stop()     {endpoint   = std::chrono::high_resolution_clock::now(); fetchtime();}
        
        void fetchtime(){
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endpoint - startpoint);
            cout << format("{}  nanoseconds \n", duration.count());
        }
        void fetchtime(string stringarg){
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endpoint - startpoint);
            cout << format("{} took {}  nanoseconds \n", stringarg, duration.count());
        }
        void measure(){
            auto temp = std::chrono::high_resolution_clock::now();
            auto nsduration  = std::chrono::duration_cast<std::chrono::nanoseconds>(temp - startpoint);
            auto msduration  = std::chrono::duration_cast<std::chrono::microseconds>(temp - startpoint);
            auto sduration   = std::chrono::duration_cast<std::chrono::seconds>(temp - startpoint);
            cout << format("{} nanoseconds | {} microseconds | {} seconds \n", 
                nsduration.count(), msduration.count(), sduration.count());
        }
        ~Timer(){
            auto temp = std::chrono::high_resolution_clock::now();
            auto nsduration  = std::chrono::duration_cast<std::chrono::nanoseconds>(temp - startpoint);
            auto msduration  = std::chrono::duration_cast<std::chrono::microseconds>(temp - startpoint);
            auto milliduration = std::chrono::duration_cast<std::chrono::milliseconds>(temp - startpoint);
            auto sduration   = std::chrono::duration_cast<std::chrono::seconds>(temp - startpoint);
            PRINTLINE
            cout << format("function-name = {} | {} nanoseconds | {} microseconds | {} milliseconds | {} seconds \n", 
                functionname, 
                nsduration.count(), 
                msduration.count(), 
                milliduration.count(), 
                sduration.count());

            // writing to the file 
            if (!filename.empty()){
                std::ofstream fileobj(filename, std::ios::app);
                if (fileobj){
                    if (functionname.empty()){
                        fileobj << "main" << "," << nsduration.count() << "," << msduration.count() << "," << sduration.count() << "\n";
                    }
                    else{
                        fileobj << functionname << "," << nsduration.count() << "," << msduration.count() << "," << sduration.count() << "\n";
                    }
                }
            }
        }
    };
}
