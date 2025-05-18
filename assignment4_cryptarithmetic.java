import java.util.*;

public class assignment4_cryptarithmetic {

    private static int wordToNumber(String word, Map<Character, Integer> map) {
        int num = 0;
        for (char ch : word.toCharArray()) {
            num = num * 10 + map.get(ch);
        }
        return num;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Take input from the user
        System.out.print("Enter the first word: ");
        String s1 = scanner.next().toUpperCase();

        System.out.print("Enter the second word: ");
        String s2 = scanner.next().toUpperCase();

        System.out.print("Enter the result word: ");
        String s3 = scanner.next().toUpperCase();

        Set<Character> uniqueChars = new HashSet<>();
        for (char c : (s1 + s2 + s3).toCharArray()) {
            uniqueChars.add(c);
        }

        if (uniqueChars.size() > 10) {
            System.out.println("Too many unique letters! Cannot solve.");
            return;
        }

        List<Character> letters = new ArrayList<>(uniqueChars);
        int[] digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        permute(digits, 0, letters, s1, s2, s3);
        System.out.println("No solution found.");
    }

    private static void permute(int[] digits, int index, List<Character> letters, String s1, String s2, String s3) {
        if (index == letters.size()) {
            Map<Character, Integer> map = new HashMap<>();
            for (int i = 0; i < letters.size(); i++) {
                map.put(letters.get(i), digits[i]);
            }

            // First letters can't be zero
            if (map.get(s1.charAt(0)) == 0 || map.get(s2.charAt(0)) == 0 || map.get(s3.charAt(0)) == 0) {
                return;
            }

            int num1 = wordToNumber(s1, map);
            int num2 = wordToNumber(s2, map);
            int num3 = wordToNumber(s3, map);

            if (num1 + num2 == num3) {
                System.out.println("\nSolution Found:");
                for (Map.Entry<Character, Integer> entry : map.entrySet()) {
                    System.out.println(entry.getKey() + " = " + entry.getValue());
                }
                System.out.println(s1 + " + " + s2 + " = " + s3);
                System.out.println(num1 + " + " + num2 + " = " + num3);
                System.exit(0);
            }
            return;
        }

        for (int i = index; i < digits.length; i++) {
            swap(digits, i, index);
            permute(digits, index + 1, letters, s1, s2, s3);
            swap(digits, i, index); // backtrack
        }
    }

    private static void swap(int[] digits, int i, int j) {
        int temp = digits[i];
        digits[i] = digits[j];
        digits[j] = temp;
    }
}
