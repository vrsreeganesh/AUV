#pragma once
namespace   svr     {
    
    template    <typename T>
    concept IFFTPlanClassSourceDestinationType = \
        std::is_floating_point_v<T> || 
        (
            std::is_class_v<T>  && 
            std::is_floating_point_v<typename T::value_type>
        );
    template    <
        IFFTPlanClassSourceDestinationType sourceType,
        IFFTPlanClassSourceDestinationType destinationType
    >
    class   IFFTPlanClass
    {
        public:
            std::size_t         nfft_;
            fftw_complex*       in_;
            fftw_complex*       out_;
            fftw_plan           plan_;
            
            /*==================================================================
            Destructor
            ------------------------------------------------------------------*/
            ~IFFTPlanClass()
            {
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}
            }
            /*==================================================================
            Constructor
            ------------------------------------------------------------------*/ 
            IFFTPlanClass(const std::size_t nfft):  nfft_(nfft)
            {
                // allocating space
                in_     =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                out_    =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                if(!in_ || !out_)     {throw std::runtime_error("in_, out_ creation failed");}

                // creating plan
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_BACKWARD,
                    FFTW_MEASURE
                );
                if(!plan_)           {throw std::runtime_error("File: FFTPlanClass.hpp | Class: IFFTPlanClass | report: plan-creation failed");}
            }
            /*==================================================================
            Copy Constructor
            ------------------------------------------------------------------*/
            IFFTPlanClass(const     IFFTPlanClass&      other)
            {
                // allocating space
                nfft_   =   other.nfft_;
                in_     =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                out_    =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                if (!in_ || !out_)  {throw std::runtime_error("FILE: FFTPlanClass.hpp | Class: IFFTPlanClass | Function: Copy-Constructor | Report: in-out plan creation failed");}

                // copying contents
                std::memcpy(in_,    other.in_,  nfft_ * sizeof(fftw_complex));
                std::memcpy(out_,   other.out_, nfft_ * sizeof(fftw_complex));

                // creating a new plan since its more of an engine
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_BACKWARD,
                    FFTW_MEASURE
                );
                if(!plan_)   {throw  std::runtime_error("FILE:   FFTPlanClass.hpp | Class: IFFTPlanClass | Function: Copy-Constructor | Report: plan-creation failed");}

            }
            /*==================================================================
            Copy Assignment
            ------------------------------------------------------------------*/ 
            IFFTPlanClass&  operator=(const IFFTPlanClass&  other)
            {
                // handling self-assignment
                if(this == &other)     {return *this;}

                // cleaning up existing resources
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}

                // allocating space
                nfft_   =   other.nfft_;
                in_     =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                out_    =   reinterpret_cast<fftw_complex*>(fftw_malloc(nfft_ * sizeof(fftw_complex)));
                if (!in_ || !out_)  {throw std::runtime_error("FILE: FFTPlanClass.hpp | Class: IFFTPlanClass | Function: Copy-Constructor | Report: in-out plan creation failed");}

                // copying contents
                std::memcpy(in_,    other.in_,  nfft_ * sizeof(fftw_complex));
                std::memcpy(out_,   other.out_, nfft_ * sizeof(fftw_complex));

                // creating a new plan since its more of an engine
                plan_   =   fftw_plan_dft_1d(
                    static_cast<int>(nfft_),
                    in_,
                    out_,
                    FFTW_BACKWARD,
                    FFTW_MEASURE
                );
                if(!plan_)   {throw  std::runtime_error("FILE:   FFTPlanClass.hpp | Class: IFFTPlanClass | Function: Copy-Constructor | Report: plan-creation failed");}

                // returning
                return *this;

            }
            /*==================================================================
            Move Constructor
            ------------------------------------------------------------------*/ 
            IFFTPlanClass(IFFTPlanClass&&   other)  noexcept
                :   nfft_(  other.nfft_),
                    in_(    other.in_),
                    out_(   other.out_),
                    plan_(  other.plan_)
            {
                // resetting the source object
                other.nfft_     =   0;
                other.in_       =   nullptr;
                other.out_      =   nullptr;
                other.plan_     =   nullptr;
            }
            /*==================================================================
            Move-Assignment
            ------------------------------------------------------------------*/ 
            IFFTPlanClass&  operator=(IFFTPlanClass&& other)    noexcept
            {
                // self-assignment check
                if(this == &other)      {return *this;}

                // cleaning up existing 
                if(plan_ != nullptr)    {fftw_destroy_plan(      plan_);}
                if(in_   != nullptr)    {fftw_free(              in_);}
                if(out_  != nullptr)    {fftw_free(              out_);}

                // Copying values and changing pointers
                nfft_       =   other.nfft_;
                in_         =   other.in_;
                out_        =   other.out_;
                plan_       =   other.plan_;

                // resetting the source-object
                other.nfft_     =   0;
                other.in_       =   nullptr;
                other.out_      =   nullptr;
                other.plan_     =   nullptr;

                // returning
                return *this;
            }
            /*==================================================================
            Running
            ------------------------------------------------------------------*/ 
            std::vector<destinationType>
            ifft(const  std::vector<sourceType>&   input_vector)
            {
                // throwing an error
                if (input_vector.size() >  nfft_)
                    throw std::runtime_error("File: FFTPlanClass | Class: IFFTPlanClass | Function: ifft() | Report: size of vector > nfft ");

                // copy input into fftw buffer
                for(std::size_t index = 0; index < nfft_; ++index)
                {
                    if  constexpr(
                        std::is_same_v<     sourceType, std::complex<double>    >   ||
                        std::is_same_v<     sourceType, std::complex<float>     >
                    ){
                        in_[index][0]   =   input_vector[index].real();
                        in_[index][1]   =   input_vector[index].imag();
                    }
                    else if constexpr(
                        std::is_floating_point_v<   sourceType  >
                    ){
                        in_[index][0]   =   input_vector[index];
                        in_[index][1]   =   0;
                    }
                }

                // execute ifft
                fftw_execute(plan_);

                // normalize output
                std::vector<destinationType>   output_vector(nfft_);
                for(std::size_t index = 0; index < nfft_; ++index){
                    if constexpr(
                        std::is_floating_point_v<   destinationType >
                    ){
                        output_vector[index]    =   static_cast<destinationType>(out_[index][0]/nfft_);
                    }
                    else if constexpr(
                        std::is_same_v<     destinationType, std::complex<double>    > ||
                        std::is_same_v<     destinationType, std::complex<float>    >
                    ){
                        output_vector[index][0] =   destinationType(
                            out_[index][0]/nfft_, 
                            out_[index][1]/nfft_
                        );
                    }
                }

                // returning
                return std::move(output_vector);
            }
            /*==================================================================
            Running - proper bases change
            ------------------------------------------------------------------*/ 
            std::vector<destinationType>
            ifft_l2_conserved(const  std::vector<sourceType>&   input_vector)
            {
                // throwing an error
                if (input_vector.size() >  nfft_)
                    throw std::runtime_error("File: FFTPlanClass | Class: IFFTPlanClass | Function: ifft() | Report: size of vector > nfft ");

                // copy input into fftw buffer
                for(std::size_t index = 0; index < nfft_; ++index)
                {
                    if  constexpr(
                        std::is_same_v<     sourceType, std::complex<double>    >   ||
                        std::is_same_v<     sourceType, std::complex<float>    >
                    ){
                        in_[index][0]   =   input_vector[index].real();
                        in_[index][1]   =   input_vector[index].imag();
                    }
                    else if constexpr(
                        std::is_floating_point_v<sourceType>
                    ){
                        in_[index][0]   =   input_vector[index];
                        in_[index][1]   =   0;
                    }
                }

                // execute ifft
                fftw_execute(plan_);

                // normalize output
                std::vector<    destinationType >   output_vector(nfft_);
                for(std::size_t index = 0; index < nfft_; ++index)
                {
                    if constexpr(
                        std::is_floating_point_v<destinationType>
                    ){
                        output_vector[index]  =  static_cast<destinationType>(out_[index][0] * 1.00/std::sqrt(nfft_));
                    }
                    else if constexpr(
                        std::is_same_v<     destinationType, std::complex<double>   >  ||
                        std::is_same_v<     destinationType, std::complex<float>    >
                    ){
                        output_vector[index] =   destinationType(
                            out_[index][0]  *   1.00/std::sqrt(nfft_), 
                            out_[index][1]  *   1.00/std::sqrt(nfft_)
                        );
                    }
                }

                // returning
                return std::move(output_vector);
            }
    };
}