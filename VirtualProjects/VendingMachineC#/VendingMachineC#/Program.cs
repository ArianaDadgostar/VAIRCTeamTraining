// See https://aka.ms/new-console-template for more information
using System;

namespace StitchingTest
{
    public static class VendingMachine
    {
        public static Dictionary<int, int> Bank = new Dictionary<int, int>()
        {
            {100, 1},
            {50, 2},
            {25, 3},
            {5, 3},
            {4, 3},
            {1, 3},
        };

        public static int GetOptimal(int amount)
        {
            int index = 0;

            for(int i = 0; i < Bank.Count; i++)
            {
                if(Bank.ElementAt(i).Key > amount) continue;

                index = i;
                break;
            }
            int coin = Bank.ElementAt(index).Key;
            int count = (amount % coin > 0) ? (int)(amount / coin) + 1 : (int)(amount / coin);

            return count;
        }

        public static int[] AnalyzePossibilities(int amount)
        {
            int coinCount = GetOptimal(amount);
            int[] combination = new int[coinCount];
            while(!EqualsAmount(amount, combination) && coinCount <= Bank.Count)
            {
                combination = new int[coinCount];
                RecursiveAnalyzation(amount, coinCount, 0, combination);
                coinCount++;
            }
            if(coinCount > Bank.Count) return null;
            return combination;
        }

        public static bool EqualsAmount(int amount, int[] combination)
        {
            int total = 0;
            foreach(int coin in combination)
            {
                total += coin;
            }
            // Use a small tolerance for floating-point comparison
            if(Math.Abs(amount - total) < 0.0001) return true; // FOR SOME REASONG TOTAL IS 0.3000000004
            return false;
        }

        public static bool CanGiveCoin(int amount, int[] combination)
        {
            int count = 0;
            foreach(int coin in combination)
            {
                if(coin != amount) continue;

                count++;
            }
            if(count >= Bank[amount]) return false;
            return true;
        }

        public static bool RecursiveAnalyzation(int amount, int coinCount, int currentIndex, int[] combination)
        {
            foreach(int coin in Bank.Keys)
            {
                if(Bank[coin] <= 0) continue;
                if(!CanGiveCoin(coin, combination)) continue;
                combination[currentIndex] = coin;
                
                if(EqualsAmount(amount, combination)) return true;
                if(currentIndex >= coinCount - 1) continue;

                if(RecursiveAnalyzation(amount, coinCount, currentIndex + 1, combination)) return true;
            }
            return false;
        }
    }

    public class Program
    {
        static int Main(string[] args)
        {
            while(true)
            {
                Console.WriteLine("How much is your change? ");
                int change = int.Parse(Console.ReadLine());
                int[] coins = VendingMachine.AnalyzePossibilities(change);
                if(coins == null)
                {
                    Console.WriteLine("No tengo ts sorry");
                    continue;
                }
                
                foreach(int coin in coins)
                {
                    Console.WriteLine(coin);
                }
            }
        }
    }
}


