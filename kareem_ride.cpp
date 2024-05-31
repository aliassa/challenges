#include <iostream>

float base_fee { 5.0f};
float distance_rate { 2.5f };
float time_rate { 0.5f };
// 60 * 24 = 1440 mintes in a day
// from 1200 -> 360 (night)

struct RideData
{
    char ride_name[100];
    float distance { 0.0f };
    bool peak { false };
    struct
    {
        uint32_t minute_start;
        uint32_t minute_end;
    } rideTimes;
    
};

float get_price(const RideData data)
{
    uint32_t start = data.rideTimes.minute_start;
    uint32_t end = data.rideTimes.minute_end;

    // Validate input
    if(start > 1440 || end > 1440)
    {
        std::cout << "Please enter a valid time (from 0 to 1440 minues)\n";
        return 0.0;
    }
    if(data.peak) base_fee *= 1.2f;
    else base_fee *= 0.9f;

    if((start >=1200 && start <= 360) || (end >= 1200 && end <= 360))
        time_rate *= 1.2;

    float tot_price = base_fee;
    if(data.distance >= 5 && data.distance <= 10)
        distance_rate *= 0.9f;
    else if (data.distance > 10)
        distance_rate *= 0.85f;
    tot_price += data.distance * distance_rate;
    // check if passed midight in-ride
    if (end == start) return base_fee;
    if(end > start)
        tot_price += (end - start) * time_rate;
    else
      // Predicate : a ride cannot take more than 24 hours.
      tot_price += ((1440 - start) + end) *  time_rate;

    return tot_price;

}

int main()
{
    const RideData data{"From home to aeroport", 20, true , { 0, 120 }};
    std::cout << "price of ["  << data.ride_name  << "] : " << get_price(data) << '\n';
    return 0;
}
