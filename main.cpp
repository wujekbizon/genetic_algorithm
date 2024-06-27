#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

// Solution class represents a single solution in the population
class Solution
{

  public:
    // Default constructor
    Solution() = default;

    // Constructor to initialize rank, x, y, and z values
    Solution(double rank, double x, double y, double z) : rank(rank), x(x), y(y), z(z)
    {
    }

    // Public methods to mutate x, y, and z with a factor
    void mutateX(double factor)
    {
        x *= factor;
    }

    void mutateY(double factor)
    {
        y *= factor;
    }
    void mutateZ(double factor)
    {
        z *= factor;
    }

    // Public getter methods to access private members
    double getX() const
    {
        return x;
    }

    double getY() const
    {
        return y;
    }

    double getZ() const
    {
        return z;
    }

    double getRank() const
    {
        return rank;
    }

    // Fitness function to calculate the rank based on the formula
    void fitness()
    {
        double answer = (6 * x + -y + std::pow(z, 200)) - 25;

        /*
          if answer is 0 we assign to rank 9999 otherwise for the number as closer to 0
          we will assign bigger number
          for example: 1/0.5 = 2.0 , 1/0.05 = 20.0 or 1/0.0005 = 2000.0
        */
        rank = (answer == 0) ? 9999 : std::abs(1 / answer);
    }

  private:
    // Rank of the solution (higher rank indicates better fitness)
    double rank;
    // Private member variables representing the solution components (x, y, z)
    double x, y, z;

    // Friend declaration to allow stream insertion operator access to private members
    friend std::ostream &operator<<(std::ostream &os, const Solution &s);
};

// Stream insertion operator overload to print Solution objects in a formatted way
inline std::ostream &operator<<(std::ostream &os, const Solution &s)
{
    os << std::fixed << "Rank " << static_cast<int>(s.rank) << std::endl;
    os << "x: " << s.x << " y: " << s.y << " z: " << s.z << std::endl;
    return os;
}

int main()
{
    // Create random device for generating random numbers
    std::random_device device;

    // Define uniform real distribution for generating initial random solutions between -100 and 100
    std::uniform_real_distribution<double> unif(-100, 100);

    // Vector to store the solutions
    std::vector<Solution> solutions;

    // Number of solutions in the population
    const int NUM = 100000;

    // Create initial random solutions
    for (size_t i = 0; i < NUM; i++)
    {
        solutions.push_back(Solution{0, unif(device), unif(device), unif(device)});
    }

    // Loop for the genetic algorithm
    while (true)
    {

        // Run fitness function on each solution
        for (auto &s : solutions)
        {
            s.fitness();
        }

        // Sort solutions by rank (higher rank -> better solution)
        std::sort(solutions.begin(), solutions.end(),
                  [](const auto &lhs, const auto &rhs) { return lhs.getRank() > rhs.getRank(); });

        // Print top 10 solutions (for debugging or visualization)
        std::for_each(solutions.begin(), solutions.begin() + 10, [](const auto &s) { std::cout << s << std::endl; });

        // Select top solutions for breeding (sample size)
        const int SAMPLE_SIZE = 1000;
        std::vector<Solution> sample;

        // Efficiently move top solutions from solutions to sample using move semantics
        std::move(solutions.begin(), solutions.begin() + SAMPLE_SIZE, std::back_inserter(sample));

        // Clear the original solutions vector to prepare for next generation
        solutions.clear();

        // Mutate the top solutions with a random factor between 0.99 and 1.01 (1% mutation)
        std::uniform_real_distribution<double> m(0.99, 1.01); // mutate at most by 1%

        std::for_each(sample.begin(), sample.end(), [&](auto &s) {
            s.mutateX(m(device)); // Apply mutation to x with a random factor from the distribution
            s.mutateY(m(device)); // Apply mutation to y with a random factor from the distribution
            s.mutateZ(m(device)); // Apply mutation to z with a random factor from the distribution
        });

        // Cross over (breeding) operation
        std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);

        // Loop to create the next generation
        for (size_t i = 0; i < NUM; i++)
        {
            // Create a new solution using the selected parents' components (x, y, z)
            solutions.push_back(
                Solution{0, sample[cross(device)].getX(), sample[cross(device)].getY(), sample[cross(device)].getZ()});
        }
    }
    return 0;
}