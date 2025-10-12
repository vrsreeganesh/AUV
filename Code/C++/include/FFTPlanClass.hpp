#pragma once

namespace   svr     {

    template    <typename T>
    concept FFTPlanClassSourceDestinationType = \
        std::is_floating_point_v<T> || 
        (
            std::is_class_v<T>  && 
            std::is_floating_point_v<typename T::value_type>
        );
    template    <
        FFTPlanClassSourceDestinationType sourceType,
        FFTPlanClassSourceDestinationType destinationType
    >
    class FFTPlanClass
    {
        public:

            // Members
            std::size_t     nfft_       {std::numeric_limits<std::size_t>::max()};
            fftw_complex*   in_         {nullptr};
            fftw_complex*   out_        {nullptr};
            fftw_plan       plan_       {nullptr};

            /*==================================================================
            Destructor
            ------------------------------------------------------------------*/
            ~FFTPlanClass()
            {
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}
            }
            /*==================================================================
            Default Constructor
            ------------------------------------------------------------------*/
            FFTPlanClass()  =   default;
            /*==================================================================
            Constructor
            ------------------------------------------------------------------*/
            FFTPlanClass(const  std::size_t     nfft)
            {

                // allocating nfft
                this->nfft_   = nfft;

                // allocating input-region
                in_     =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                out_    =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                if(!in_ || !out_)   {throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | REPORT: in-out allocation failed");}

                // creating plan
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_FORWARD,
                    FFTW_MEASURE
                );
                if(!plan_)          {throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | REPORT: plan-creation failed");}
            }
            /*==================================================================
            Copy Constructor
            ------------------------------------------------------------------*/
            FFTPlanClass(const  FFTPlanClass&   other)
            {
                // copying nfft
                nfft_   =   other.nfft_;
                cout << format("\t\t FFTPlanClass(const  FFTPlanClass&   other) | nfft_ = {}\n", nfft_);

                // allocating input-region
                in_     =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                out_    =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                if(!in_ || !out_)   {throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | REPORT: in-out allocation failed");}

                // copying input-region and output-region
                std::memcpy(in_,    other.in_,  nfft_   * sizeof(fftw_complex));
                std::memcpy(out_,   other.out_, nfft_   *   sizeof(fftw_complex));

                // creating plan
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_FORWARD,
                    FFTW_MEASURE
                );
                if(!plan_)          {throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | REPORT: plan-creation failed");}
            }
            /*==================================================================
            Copy Assignment
            ------------------------------------------------------------------*/
            FFTPlanClass&   operator=(const     FFTPlanClass&   other)
            {
                // handling self-assignment
                if  (this  ==  &other)     {return *this;}

                // cleaning-up existing resources
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}

                // allocating input-region and output-region
                nfft_   =   other.nfft_;
                in_     =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                out_    =   reinterpret_cast<fftw_complex*>(
                    fftw_malloc(nfft_   *   sizeof(fftw_complex))
                );
                if(!in_ || !out_)     {throw  std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | FUNCTION: Copy-Assignment | REPORT: in-out allocation failed");}

                // copying contents
                cout << format("\t\t FFTPlanClass&   operator=(const     FFTPlanClass&   other) | nfft_ = {}\n", nfft_);
                std::memcpy(in_,  other.in_,  nfft_ * sizeof(fftw_complex));
                std::memcpy(out_, other.out_, nfft_ * sizeof(fftw_complex));

                // creating engine
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_FORWARD,
                    FFTW_MEASURE
                );
                if(!plan_)   {throw  std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | FUNCTION: Copy-Assignment | REPORT: plan-creation failed");}

                // returning
                return *this;
            }
            /*==================================================================
            Move Constructor
            ------------------------------------------------------------------*/
            FFTPlanClass(FFTPlanClass&& other)
                :   nfft_(              other.nfft_),
                    in_(                other.in_),
                    out_(               other.out_),
                    plan_(              other.plan_)
            {
                // resetting the others
                other.nfft_         =   0;
                other.in_           =   nullptr;
                other.out_          =   nullptr;
                other.plan_         =   nullptr;
            }
            /*==================================================================
            Move Assignment
            ------------------------------------------------------------------*/
            FFTPlanClass&   operator=(FFTPlanClass&& other)
            {
                // self-assignment check
                if  (this   ==  &other)     {return *this;}

                // cleaning up existing resources
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}

                // Copying-values and changing pointers
                nfft_               =   other.nfft_;
                cout << format("\t\t FFTPlanClass's MOVE assignment | nfft_ = {}\n", nfft_);
                in_                 =   other.in_;
                out_                =   other.out_;
                plan_               =   other.plan_;

                // resetting source-members
                other.nfft_         =   0;
                other.in_           =   nullptr;
                other.out_          =   nullptr;
                other.plan_         =   nullptr;

                // returning 
                return *this;
            }
            /*==================================================================
            Running fft
            ------------------------------------------------------------------*/
            std::vector<destinationType>
            fft(const   std::vector<sourceType>&    input_vector)
            {
                // throwing an error
                if  (input_vector.size()    >   nfft_){
                    cout << format("input_vector.size() = {}, nfft_ = {}\n",
                                   input_vector.size(),
                                    nfft_);
                    throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | FUNCTION: fft() | REPORT: input-vector size is greater than NFFT");
                }
                    

                // copying inputs 
                for(std::size_t index = 0; index < input_vector.size(); ++index)
                {
                    if  constexpr(
                        std::is_floating_point_v<sourceType>
                    ){
                        in_[index][0]   =   input_vector[index];
                        in_[index][1]   =   0;
                    }
                    else if constexpr(
                        std::is_same_v<sourceType,  std::complex<float>>    ||
                        std::is_same_v<sourceType,  std::complex<double>>
                    ){
                        in_[index][0]   =   input_vector[index].real();
                        in_[index][1]   =   input_vector[index].imag();
                    }
                }

                // executing fft
                fftw_execute(plan_);

                // copying results to output-vector
                std::vector<destinationType>    output_vector(nfft_);
                for(std::size_t index = 0; index < nfft_; ++index){
                    if  constexpr(
                        std::is_same_v<destinationType,  std::complex<float>>    ||
                        std::is_same_v<destinationType,  std::complex<double>>
                    ){
                        output_vector[index]    =   destinationType(
                            out_[index][0],
                            out_[index][1]
                        );
                    }
                    else if constexpr(
                        std::is_floating_point_v<destinationType>
                    ){
                        output_vector[index]    =   static_cast<destinationType>(
                            std::sqrt(
                                std::pow(out_[index][0], 2)  +   \
                                std::pow(out_[index][1], 2)
                            )
                        );
                    }
                }

                // returning output
                return std::move(output_vector);
            }
            /*==================================================================
            Running fft - balanced
            ------------------------------------------------------------------*/
            std::vector<destinationType>
            fft_l2_conserved(const   std::vector<sourceType>&    input_vector)
            {
                // throwing an error
                if  (input_vector.size()    >   nfft_)
                    throw std::runtime_error("FILE: FFTPlanClass.hpp | CLASS: FFTPlanClass | FUNCTION: fft() | REPORT: input-vector size is greater than NFFT");

                // copying inputs 
                for(std::size_t index = 0; index < input_vector.size(); ++index)
                {
                    if  constexpr(
                        std::is_floating_point_v<sourceType>
                    ){
                        in_[index][0]   =   input_vector[index];
                        in_[index][1]   =   0;
                    }
                    else if constexpr(
                        std::is_same_v<sourceType, std::complex<float>> ||
                        std::is_same_v<sourceType, std::complex<double>>
                    ){
                        in_[index][0]   =   input_vector[index].real();
                        in_[index][1]   =   input_vector[index].imag();
                    }
                }

                // executing fft
                fftw_execute(plan_);

                // copying results to output-vector
                std::vector<destinationType>    output_vector(nfft_);
                for(std::size_t index = 0; index < nfft_; ++index)
                {
                    if  constexpr(
                        std::is_same_v<     destinationType, std::complex<double>    >    ||
                        std::is_same_v<     destinationType, std::complex<float>    >
                    ){
                        output_vector[index]    =   destinationType(
                            out_[index][0] * (1.00 / std::sqrt(nfft_)),
                            out_[index][1] * (1.00 / std::sqrt(nfft_))
                        );
                    }
                    else if constexpr(
                        std::is_floating_point_v<destinationType>
                    ){
                        output_vector[index]    =   destinationType(
                            std::sqrt(
                                std::pow(out_[index][0] * (1.00 / std::sqrt(nfft_)), 2)  +   \
                                std::pow(out_[index][1] * (1.00 / std::sqrt(nfft_)), 2)
                            )
                        );
                    }
                }

                // returning output
                return std::move(output_vector);
            }      
    };
}