// See https://aka.ms/new-console-template for more information
using System;

namespace StitchingTest
{
    public static class VendingMachine
    {
        public static double[] coins = { 1.00, 0.5, 0.25, 0.1, 0.05, 0.01 };
        public static Dictionary<double, int> Bank = new Dictionary<double, int>()
        {
            {1.00, 1},
            {0.5, 2},
            {0.25, 3},
            {0.1, 3},
        };

        public static int GetOptimal(double amount)
        {
            int index = 0;

            for(int i = 0; i < coins.Length; i++)
            {
                if(coins[i] > amount) continue;

                index = i;
                break;
            }
            int count = (amount % coins[index] > 0) ? (int)(amount / coins[index]) + 1 : (int)(amount / coins[index]);

            return count;
        }

        public static double[] AnalyzePossibilities(double amount)
        {
            int coinCount = GetOptimal(amount);
            double[] combination = new double[coinCount];
            while(!EqualsAmount(amount, combination) && coinCount <= Bank.Count)
            {
                combination = new double[coinCount];
                RecursiveAnalyzation(amount, coinCount, 0, combination);
                coinCount++;
            }
            if(coinCount > Bank.Count) return null;
            return combination;
        }

        public static bool EqualsAmount(double amount, double[] combination)
        {
            double total = 0;
            foreach(double coin in combination)
            {
                total += coin;
            }
            // Use a small tolerance for floating-point comparison
            if(Math.Abs(amount - total) < 0.0001) return true; // FOR SOME REASONG TOTAL IS 0.3000000004
            return false;
        }

        public static void RecursiveAnalyzation(double amount, int coinCount, int currentIndex, double[] combination)
        {
            foreach(double coin in Bank.Keys)
            {
                if(Bank[coin] <= 0) continue;
                combination[currentIndex] = coin;
                
                if(EqualsAmount(amount, combination)) return;
                if(currentIndex >= coinCount - 1) continue;

                RecursiveAnalyzation(amount, coinCount, currentIndex + 1, combination);
            }
        }
    }

    public class Program
    {
        static int Main(string[] args)
        {
            while(true)
            {
                Console.WriteLine("How much is your change? ");
                double change = double.Parse(Console.ReadLine());
                double[] coins = VendingMachine.AnalyzePossibilities(change);
                if(coins == null)
                {
                    Console.WriteLine("No tengo ts sorry");
                    continue;
                }
                
                foreach(double coin in coins)
                {
                    Console.WriteLine(coin);
                }
            }
        }
    }
}


