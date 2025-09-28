/*==============================================================================
Class Declaration
-------------------------------------------------------------------------------*/
template <typename T>
class ScattererClass
{
public:
    // members
    std::vector<std::vector<T>> coordinates;
    std::vector<T>              reflectivity;

    // Constructor
    ScattererClass()    {}

    // Constructor
    ScattererClass(std::vector<std::vector<T>> coordinates_arg,
                   std::vector<T>              reflectivity_arg):
                    coordinates(std::move(coordinates_arg)),
                    reflectivity(std::move(reflectivity_arg))   {}

    // Save to CSV
    void save_to_csv();
};
/*==============================================================================
Saving to CSV-Files
-------------------------------------------------------------------------------*/
template    <typename   T>
void    ScattererClass<T>::save_to_csv()
{
    fWriteMatrix(this->coordinates,   "../csv-files/coordinates.csv");
    fWriteVector(this->reflectivity,  "../csv-files/reflectivity.csv");
}