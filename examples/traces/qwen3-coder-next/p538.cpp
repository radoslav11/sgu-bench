#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <vector>

int main() {
    std::string s;
    std::getline(std::cin, s);
    
    int n = s.length();
    std::vector<bool> is_emoticon_bracket(n, false);
    std::stack<int> bracket_stack;
    
    // First pass: identify brackets that form emoticons according to the rules
    // We'll mark brackets that are part of emoticons
    
    // We need to find matching brackets that satisfy the condition:
    // - opening bracket '(' followed by a closing bracket ')' with only spaces/letters between
    // But note: the problem says that "()" is NOT part of emoticons.
    // However, the definition is: a bracket is part of an emoticon if it satisfies certain conditions.
    // Let's re-read the rules carefully.
    
    // Rule:
    // An opening bracket '(' is part of an emoticon if:
    //   there exists the nearest bracket following to the right, which is a closing bracket ')',
    //   and there shouldn't be anything between the brackets but spaces and Latin letters.
    //
    // A closing bracket ')' is part of an emoticon if:
    //   there exists the nearest round bracket to the left, which is an opening bracket '(',
    //   and there shouldn't be anything between the brackets but spaces and Latin letters.
    //
    // Note: if a bracket doesn't satisfy the condition, it IS considered part of an emoticon.
    // Wait, no: "If a bracket doesn't satisfy the conditions, it is considered a part of an emoticon."
    // This seems contradictory with the example. Let me read again.
    
    // Actually, the problem says:
    // "a round opening or closing bracket be considered part of an emoticon if: [condition]"
    // and then:
    // "If a bracket doesn't satisfy the conditions, it is considered a part of an emoticon."
    // That can't be right. Let me re-read the original.
    
    // Original: "a round opening or closing bracket be considered part of an emoticon if: ... 
    // If a bracket doesn't satisfy the conditions, it is considered a part of an emoticon."
    // This must be a translation issue. Looking at the example:
    // "Hi:) (it is me) I have bad news:-(("
    // Only the brackets that outline "it is me" aren't emoticons.
    // So in "Hi:)" the ':' and ')' are not part of the emoticon condition? 
    // But note: the example says only the brackets around "it is me" are NOT emoticons.
    // So all other brackets ARE emoticons.
    
    // Let me break down the example:
    // String: "Hi:) (it is me) I have bad news:-(("
    // Brackets positions (0-indexed):
    //   H i : )   ( i t   i s   m e )   ...  : - ( ( 
    // Positions: 0 1 2 3 4 5 6 ...
    // At position 3: ')' - is it part of an emoticon? Yes (because the rule says it's an emoticon unless it's part of the "it is me" pair)
    // At position 5: '(' - is it part of an emoticon? No, because it forms a valid emoticon pair with the ')' at position 14 (or so)
    // But wait, the problem says: "the brackets that outline 'it is me' aren't emoticons"
    // So '(' at position 5 and ')' at position 14 are NOT emoticons.
    // All others are.
    
    // How does the rule apply to "Hi:)"?
    // For the ')' at position 3: 
    //   To be part of an emoticon as a closing bracket, we look for the nearest round bracket to the left, which is '('.
    //   But where is the '('? It's not there. The '(' is not present before position 3 in "Hi:)".
    //   Actually, in "Hi:)", the only brackets are at position 3: ')'.
    //   So for this ')', there is no nearest round bracket to the left -> condition fails -> so by the rule it is part of an emoticon.
    
    // For ":-(": 
    //   The string has ": - ( ("
    //   Let's say positions: ..., ':', '-', '(', '('
    //   For the first '(' at position i: 
    //      We look for the nearest round bracket to the right: it's another '(' at i+2? No, because between i and i+2 there's a '-'.
    //      So the nearest round bracket to the right is not a closing bracket? Actually, the next bracket is '(' which is not closing.
    //      So condition fails -> it is part of an emoticon.
    //   For the second '(' at position i+2: 
    //      The nearest round bracket to the right? There is none (if that's the end), but the string continues? 
    //      Actually the example string is ":-(((" so there are two '(' at the end.
    //      But in the example input: ":-((" (two opening brackets at the end).
    //      For the first '(' of the two: the nearest round bracket to the right is the next '(', which is not closing -> condition fails -> part of emoticon.
    //      For the second '(': no bracket to the right -> condition fails -> part of emoticon.
    //   So both '(' are part of emoticons.
    
    // But the example output is 3.
    // Let's count the brackets in "Hi:) (it is me) I have bad news:-((":
    //   "Hi:)" -> one ')' -> counted as emoticon? Yes -> 1
    //   "(it is me)" -> two brackets, but they are not emoticons -> 0
    //   ":-((" -> two '(' -> both counted as emoticons? Then total 1+2=3 -> matches.
    
    // However, note that in ":-((" there are two opening brackets. So 1 (from Hi:) + 2 (from :-() = 3.
    
    // But wait: what about the closing bracket in "Hi:)"? 
    // The string is "Hi:)" -> positions: 
    //   0: 'H', 1: 'i', 2: ':', 3: ')'
    // For the ')' at 3: 
    //   Condition for closing bracket: there must be a nearest round bracket to the left (which is '(') and between them only spaces and letters.
    //   But there is no '(' before the ')' in "Hi:)"! So condition fails -> it is part of an emoticon. Correct.
    
    // Now, what about the string "Hi:()"?
    //   Here, we have two brackets: '(' at some position and ')' at next, then another ')'
    //   Actually, "Hi:()" would be: 
    //      H i : ( ) 
    //   For the '(' at position 3: 
    //      nearest bracket to the right is ')' at position 4, and between them nothing (only empty) -> but the rule says "there shouldn't be anything between the brackets but spaces and Latin letters" -> empty is allowed? 
    //      However, the note says: "an opening bracket immediately followed by a closing bracket, i.e. '()', are not parts of emoticons by definition."
    //   So for the '(' at 3: the condition requires that the nearest bracket to the right is ')' and between is only spaces/letters -> but empty is allowed? 
    //      The problem states: "there shouldn't be anything between the brackets but spaces and Latin letters" -> empty string satisfies that (no non-space, non-letter).
    //   But the note explicitly says "()", i.e. two consecutive brackets, are not part of emoticons.
    //   Therefore, for the pair "()", the brackets are NOT part of emoticons.
    //   So in "Hi:()", the two brackets in "()" are not emoticons. But what about the other brackets? There are none. So count=0.
    
    // How do we reconcile? 
    // The rule says: 
    //   For an opening bracket '(' at position i: 
    //      Let j be the nearest round bracket to the right (so j>i, and there's no round bracket between i and j).
    //      Then, if s[j]==')' and the substring s[i+1:j] consists only of spaces and letters, then it satisfies the condition -> so it is part of an emoticon? 
    //   But wait: the note says "()", i.e. when there is nothing between, then they are not part of emoticons.
    //   So the condition in the problem statement must be interpreted as: 
    //        the condition described is the condition for being *not* part of an emoticon? 
    //   Actually, the problem says: "a round opening or closing bracket be considered part of an emoticon if: [condition]"
    //   and then the note says: "()", i.e. when the condition holds for a pair of consecutive brackets, they are not part of emoticons? 
    //   This is confusing.
    
    // Let me read the original problem statement again: 
    // "a round opening or closing bracket be considered part of an emoticon if:
    //   this is an opening bracket and there exists the nearest bracket following to the right. The nearest round bracket to the right should be a closing bracket and there shouldn't be anything between the brackets but spaces and Latin letters,
    //   or else it can be a closing bracket and there exists the nearest round bracket following to the left. The nearest round bracket to the left should be an opening bracket. Besides, there shouldn't be anything between the brackets but spaces and Latin letters.
    //
    // If a bracket doesn't satisfy the conditions, it is considered a part of an emoticon."
    
    // This is paradoxical: the condition describes when it is part of an emoticon, and then says if it doesn't satisfy, it is part of an emoticon? 
    // That would mean ALL brackets are part of emoticons.
    
    // But the example says: only the brackets that outline "it is me" aren't emoticons.
    // So the intended meaning must be: 
    //   A bracket is part of an emoticon if it satisfies the condition, EXCEPT for the case of "()", which is explicitly excluded.
    //   However, the problem statement says: "Note that an opening bracket immediately followed by a closing bracket, i.e. '()', are not parts of emoticons by definition."
    
    // Therefore, the condition as stated in the problem is the condition for being part of an emoticon, but with the exception that "()" is not part of emoticons.
    // So: 
    //   A bracket is part of an emoticon if:
    //      - It is an opening bracket and the nearest bracket to the right is a closing bracket and the content between is only spaces/letters, 
    //        AND it is not the case that the content between is empty? 
    //   But the note says "()" is not part of emoticons, meaning when there is nothing between, then even though the condition holds, we exclude.
    
    // Alternatively, the problem might mean: 
    //   The condition described is for being part of an emoticon, but the note says that "()" is an exception: even though the condition holds (because between is empty, which is allowed), it is not part of emoticons.
    
    // Therefore, the rule is: 
    //   A bracket is part of an emoticon if the condition holds AND the brackets are not consecutive (i.e., there is at least one non-space, non-letter? No, wait: the condition allows spaces and letters, but the note says "()" is excluded).
    //   Actually, the note says: "an opening bracket immediately followed by a closing bracket, i.e. '()', are not parts of emoticons by definition." 
    //   So if the opening bracket is immediately followed by a closing bracket (with nothing in between), then that pair is excluded.
    
    // How about " (a) "? 
    //   '(' at i, then 'a' (a letter), then ')' at j. 
    //   The condition holds: for '(' at i, the nearest bracket to the right is ')' at j, and between is 'a' (a letter) -> valid -> part of emoticon.
    //   Similarly, the ')' at j: the nearest bracket to the left is '(' at i, and between is 'a' -> valid -> part of emoticon.
    
    // So the only exception is when the opening bracket is immediately followed by a closing bracket (no characters in between).
    
    // Therefore, algorithm:
    //   We want to count the number of brackets that are part of emoticons.
    //   Total brackets that are part of emoticons = total brackets - brackets that are NOT part of emoticons.
    //   Which brackets are NOT part of emoticons?
    //      The problem says: the brackets that outline "it is me" are not emoticons. 
    //      And also, the note says "()" is not part of emoticons.
    //   So the brackets that are not part of emoticons are exactly the brackets that form a valid emoticon pair with non-empty content? 
    //      No, wait: the problem states that the brackets that outline "it is me" are not emoticons, and "it is me" has non-empty content.
    //   But then what is the difference between "a)" and "()"?
    //   Actually, the example: 
    //        "Hi:)" -> the ')' is part of an emoticon, so the brackets that are not emoticons are only the ones in the middle of the string: the pair around "it is me".
    //   So the brackets that are NOT part of emoticons are those that form a pair where the opening bracket has a matching closing bracket as the nearest bracket to the right (or vice versa) and the content is only spaces and letters, AND the pair is not the empty pair "()"?
    //   But the note says "()" is not part of emoticons, meaning it is excluded. And the example says the pair for "it is me" is also excluded.
    //   Therefore, the brackets that are NOT part of emoticons are exactly the pairs that satisfy the condition (i.e., the nearest bracket is matching and the content is spaces/letters) regardless of whether the content is empty or not? 
    //   But wait: the note says "()", i.e. empty, is not part of emoticons. And the example says the pair for "it is me" is not part of emoticons. 
    //   So all pairs that satisfy the condition (including empty) are NOT part of emoticons? 
    //   Then why is "Hi:)" having the ')' as part of an emoticon? 
    //   Because in "Hi:)", the ')' does not have a matching '(' to the left? 
    //   Let's check: 
    //        In "Hi:)", for the ')', we look for the nearest round bracket to the left: there is none -> so condition fails -> so it is part of an emoticon.
    //   In "(it is me)", for the '(', the nearest bracket to the right is ')' and between is "it is me" (only letters and spaces) -> condition holds -> so by the problem statement it would be part of an emoticon, but the note says that's not the case? 
    //   Actually, the problem says: "a round opening or closing bracket be considered part of an emoticon if: [condition]" 
    //   and then the note says: "()", meaning that even when the condition holds for a pair, if it's empty then it's excluded. 
    //   But for non-empty, is it included or excluded?
    
    // Rethink the example: 
    //   The problem says: "In the string only the brackets that outline 'it is me' aren't emoticons."
    //   So the brackets that outline "it is me" are not emoticons. 
    //   And the note says: "()", i.e. empty, are not emoticons.
    //   Therefore, the brackets that are not emoticons are exactly the pairs that satisfy the condition (i.e., the nearest bracket is matching and the content is spaces/letters) for any content (including empty).
    
    // Why then is the ')' in "Hi:)" considered an emoticon? 
    //   Because the condition for the ')' fails (no '(' to the left), so it doesn't get excluded -> it is an emoticon.
    //   Similarly, the '(' in ":-(" (the first one in ":-((") has no matching ')' as the nearest bracket to the right (the next bracket is '('), so condition fails -> it is an emoticon.
    
    // Therefore, the brackets that are NOT part of emoticons are exactly those that form a "valid" pair as described: 
    //   - '(' at i and ')' at j (j>i) such that j is the nearest bracket to the right of i, and the substring s[i+1:j] contains only spaces and letters (or is empty? but the note says empty is excluded too) -> so all such pairs are excluded.
    
    // So we need to find all pairs of brackets (i, j) such that:
    //   s[i] == '(' and s[j] == ')'
    //   and there is no bracket between i and j (so j is the next bracket after i)
    //   and the substring s[i+1:j] consists only of spaces and letters (or is empty) -> but note: the problem says "there shouldn't be anything between the brackets but spaces and Latin letters", so we check that.
    //   Then, both s[i] and s[j] are NOT part of emoticons.
    
    // However, note: the problem says "the nearest round bracket" — so we only consider the very next bracket, not the matching one in the usual sense.
    
    // Therefore, we can do:
    //   Traverse the string and for each opening bracket, check the next bracket (skipping spaces and letters? no, because brackets are only '(', ')', and the string may have other characters? but the problem says the string consists of letters, spaces, '(', ')', and punctuation: '-', ':', ',', ';'. So between two brackets, there might be punctuation.
    //   But the condition requires that between the brackets there is only spaces and letters. So if there is a punctuation mark, then the condition fails.
    
    // How to check: 
    //   For an opening bracket at i, let j be the next bracket ( scanning forward until we find a '(' or ')', and we skip non-bracket characters? but the problem says "nearest round bracket", so we skip non-round-bracket characters).
    //   Actually, the problem says: "the nearest bracket following to the right" — meaning the next round bracket (ignoring non-bracket characters? or including?).
    //   But the input may contain other characters, but the brackets are only '(' and ')'. 
    //   So the "nearest bracket" means the next occurrence of '(' or ')' to the right.
    
    // Steps:
    //   We can precompute for each bracket the next bracket to the right and the previous bracket to the left.
    
    // Alternative approach:
    //   Since the condition only depends on the immediate next bracket for an opening bracket (and immediate previous for a closing bracket), we can do:
    //      For each opening bracket at i:
    //          j = i+1
    //          while j < n and s[j] is not '(' and not ')', j++
    //          if j < n and s[j]==')' and the substring s[i+1:j] has only letters and spaces -> then this pair (i, j) is a non-emoticon pair.
    //      Similarly, for each closing bracket at j:
    //          i = j-1
    //          while i>=0 and s[i] is not '(' and not ')', i--
    //          if i>=0 and s[i]=='(' and the substring s[i+1:j] has only letters and spaces -> then this pair (i, j) is a non-emoticon pair.
    //   But note: the same pair might be detected twice (once from '(' and once from ')'). So we can mark both brackets as non-emoticon and then avoid double counting.
    
    // However, the problem: the same pair will be detected by both the '(' and the ')', so we need to mark both as non-emoticon and then count the non-emoticon brackets, then subtract from total.
    
    // Plan:
    //   Step 1: Count total number of brackets (both '(' and ')')
    //   Step 2: Identify all non-emoticon brackets (i.e., the brackets that form a valid pair as described)
    //   Step 3: Answer = total_brackets - non_emoticon_brackets_count
    
    // But note: a bracket might be part of two pairs? Unlikely because we are only considering the nearest bracket. 
    //   For example, in "(()", for the first '(':
    //        next bracket is '(' at i+1 -> so condition fails (because next is '(' not ')') -> so first '(' is emoticon.
    //   For the second '(' at i+1:
    //        next bracket is ')' at i+2 -> and between is empty -> so condition holds? but the note says "()" is not emoticon -> so non-emoticon.
    //   Then the ')' at i+2: 
    //        previous bracket is '(' at i+1 -> and between is empty -> condition holds -> non-emoticon.
    //   So the pair (i+1, i+2) is non-emoticon.
    //   Total brackets = 3, non-emoticon = 2, so answer = 1.
    
    // Example: "((two plus two equals four))" -> output 2.
    //   Let's break down: 
    //        The string: "((two plus two equals four)) "
    //        Actually, the input is: "((two plus two equals four)) " (with a space at the end? the example input says: "((two plus two equals four)) " with a space at the end? but the problem says the string does not begin or end with space, so probably no trailing space? but the example input has a space at the end in the example block? 
    //        However, the problem states: "The string does not begin with and does not end with a space."
    //        So the example input is: "((two plus two equals four))" (without trailing space).
    //        Brackets: 
    //            positions: 
    //                0: '('
    //                1: '('
    //                ... 
    //                last two: ')' at n-2 and ')' at n-1.
    //        For the inner pair: 
    //            '(' at 1: next bracket is '(' at 2? no, wait: the next bracket after 1 is the next '('? Actually, the string: 
    //                s[0]='(', s[1]='(', then "two...", then s[n-2]=')', s[n-1]=')'
    //            For s[1]='(': the next bracket is s[n-2]? but there is a bracket in between? no, the next bracket after 1 is the next '('? no, the next bracket is the first ')' at n-2? 
    //            Actually, the next bracket to the right of index1 is the next occurrence of '(' or ')'. Since after index1 we have letters, then the next bracket is the first ')' at the end. But there are two ')' at the end. 
    //            However, the condition: the nearest bracket to the right of index1 is the first ')' at n-2? 
    //            But between index1 and n-2, there is a lot of text and then the last '('? no, there is no '(' after index1 until the ')' at the end. 
    //            So for index1: 
    //                next bracket: we scan from index1+1 until we find a bracket: that will be the ')' at n-2? 
    //                But wait, the string has two '(' at the beginning and two ')' at the end. 
    //            Actually, the next bracket after index1 (which is the second '(') is the first ')' at n-2? 
    //            However, between index1 and n-2, there is the entire string "two plus two equals four" and then the last '('? no, there is no '(' in between. So the next bracket after index1 is the ')' at n-2.
    //            Now, check the substring s[2:n-2]: it is "two plus two equals four)"? no, wait: from index1+1 to n-3? 
    //            Actually, the substring from index1+1 to n-3 is "two plus two equals four", which is only letters and spaces -> valid.
    //            So the pair (1, n-2) is a non-emoticon pair? 
    //            Similarly, for the ')' at n-1: 
    //                previous bracket: scan left until we find a bracket: that's the '(' at 0? 
    //                But between 0 and n-1: the entire string, which includes the '(' at 1 and the ')' at n-2. 
    //                However, the condition requires that the nearest bracket to the left is the matching one? and between should be only spaces and letters. 
    //                But between 0 and n-1 there are brackets (the ones at 1 and n-2) which are not spaces or letters -> so condition fails for the pair (0, n-1).
    //            Now, for the inner pair: 
    //                For the '(' at 1: the next bracket is the ')' at n-2? but wait, there is a '(' at 0 to the left, but we are going right. 
    //                Actually, the next bracket after index1 is the next '(' or ')'. Since after index1 we have letters, then the next bracket is the first ')' which is at n-2. 
    //                However, the string has two ')' at the end, so the first ')' is at n-2. 
    //                And the substring from index1+1 to n-3 is the text, which is valid. 
    //                So (1, n-2) is non-emoticon.
    //            For the '(' at 0: 
    //                next bracket: the next '(' at index1? -> but that's '(' -> condition fails (because we require the next bracket to be ')') -> so it doesn't form a non-emoticon pair via the opening bracket rule.
    //            For the ')' at n-1: 
    //                previous bracket: scan left until we find a bracket: 
    //                    n-2 is ')', skip? no, we skip non-brackets? but we are looking for round brackets. 
    //                    Actually, we look for the nearest round bracket to the left: 
    //                         starting from n-2: it's a ')', so we skip? no, the problem says "round bracket", so we consider all brackets. 
    //                    But the rule for a closing bracket: we look for the nearest round bracket to the left. 
    //                    So from n-1, going left: 
    //                         n-2: ')' -> but that's not '(' -> so we continue? 
    //                         then we hit n-3: which is a letter? no, we are scanning for brackets. 
    //                    Actually, the next bracket to the left is the '(' at 1? but wait, there's a '(' at 0? 
    //                    The brackets in order: 0, 1, n-2, n-1.
    //                    For n-1: 
    //                         left: n-2 (bracket, but ')') -> skip? no, the problem says "round bracket", so we consider any '(' or ')'. 
    //                         But the condition says: the nearest round bracket to the left should be an opening bracket. 
    //                         So we want the nearest bracket that is '('.
    //                    How do we find it? 
    //                         We scan left until we find a '('.
    //                    From n-1, go left: 
    //                         n-2: ')' -> skip
    //                         n-3: letter -> skip
    //                         ... until we hit the '(' at n-2? no, the next bracket after n-2 going left is n-3? 
    //                    Actually, we skip non-bracket characters, but we stop at the first bracket. 
    //                    The first bracket to the left of n-1 is n-2, which is ')', so we continue? 
    //                    But the problem doesn't say we skip non-matching brackets. It says "the nearest round bracket", meaning the closest one regardless of type? 
    //                    However, the condition requires that the nearest round bracket is an opening bracket. 
    //                    So if the nearest round bracket is a closing bracket, then the condition fails.
    //                    Therefore, for the ')' at n-1, the nearest round bracket to the left is the ')' at n-2, which is not '(' -> condition fails.
    //            So only the pair (1, n-2) is non-emoticon? 
    //            Then non-emoticon brackets = 2 (the '(' at 1 and ')' at n-2).
    //            Total brackets = 4, so answer = 4-2 = 2. -> matches.
    
    // Implementation:
    //   Step 1: Precompute the next bracket for every bracket (for '(') and previous bracket for every bracket (for ')').
    //   However, we can do:
    //        Create an array `next_bracket` and `prev_bracket` but that might be heavy.
    //   Alternatively, we can do a linear scan to record all bracket positions, then for each bracket, find the next bracket by scanning forward until we find a bracket.
    
    // Since n<=10^5, and the number of brackets is at most n, we can do:
    //   Step 1: Extract all bracket positions and their type.
    //   Step 2: For each opening bracket at position i:
    //        Find the next bracket after i: we can iterate through the list of bracket positions until we find the first one after i.
    //        Let j = next bracket position after i.
    //        If the bracket at j is ')' and the substring s[i+1:j] consists only of letters and spaces -> then mark i and j as non-emoticon.
    //   Similarly, for each closing bracket at position j:
    //        Find the previous bracket before j: the last bracket position before j.
    //        Let i = that bracket position.
    //        If the bracket at i is '(' and the substring s[i+1:j] consists only of letters and spaces -> then mark i and j as non-emoticon.
    //   But note: the same pair will be detected twice, so we can use a boolean array `is_non_emoticon` of size n, initially false.
    //        When we find a valid pair (i, j), set is_non_emoticon[i] = true and is_non_emoticon[j] = true.
    //   Then total non-emoticon brackets = count of true in is_non_emoticon.
    //   Total brackets = count of '(' and ')' in s.
    //   Answer = total_brackets - non_emoticon_count.
    
    // However, the condition for a closing bracket might detect a pair that the opening bracket already detected? But it's the same pair, so marking twice is harmless.
    
    // Steps:
    //   1. Count total brackets and record bracket positions (index and char).
    //   2. Create a vector `brackets` of pairs: (index, char)
    //   3. Create a vector `non_emoticon` of bool, size n, initialized to false.
    //   4. For each bracket in `brackets` that is '(', do:
    //        Find the next bracket in the list (if exists) -> let it be at position j.
    //        If the bracket at j is ')', then check the substring s[i+1:j] for non-alphanumeric (but only spaces and letters allowed).
    //        How to check: for k from i+1 to j-1, if s[k] is not ' ' and not (isalpha(s[k])) -> then invalid.
    //        But note: the problem says "spaces and Latin letters", so we check:
    //            for (int k = i+1; k < j; k++) {
    //                if (s[k] != ' ' && !isalpha(s[k])) {
    //                    valid = false; break;
    //                }
    //            }
    //        If valid, set non_emoticon[i]=true, non_emoticon[j]=true.
    //   5. Similarly, for each bracket in `brackets` that is ')', do:
    //        Find the previous bracket in the list (if exists) -> let it be at position i.
    //        If the bracket at i is '(', then check the substring s[i+1:j] as above.
    //        If valid, set non_emoticon[i]=true, non_emoticon[j]=true.
    //
    // But note: step 4 and 5 might mark the same pair twice. That's okay.
    //
    // However, we can do only step 4: because for a pair (i, j) with i<j, the opening bracket i will find j as the next bracket, and the closing bracket j will find i as the previous bracket. But if we do step 4, we mark the pair. Step 5 would also mark the same pair. To avoid double work, we can do only one of them. 
    //   But what if the pair is (i, j) and the next bracket for i is j, but the previous bracket for j is i? Then step 4 and 5 are redundant.
    //   Since we are only marking booleans, it's safe to do both, but we can also do only step 4 (for opening brackets) and that will cover all pairs.
    //
    // Why? Because every non-emoticon pair must have an opening bracket that is followed immediately (in bracket terms) by a closing bracket. So step 4 will catch it.
    //   Step 5 is redundant? Actually, if we do step 4, we get the pair. Step 5 might catch the same pair again, but it's the same pair. 
    //   However, it's possible that for a closing bracket, the previous bracket is not the one that forms the pair? But the condition for the closing bracket requires the nearest bracket to the left to be '(' and the content condition. 
    //   But the nearest bracket to the left for a closing bracket j might be an opening bracket i, but there might be another opening bracket between i and j? 
    //   Example: "(()" -> for the ')' at the end, the nearest bracket to the left is '(' (the second one), so it would form a pair with the second '('.
    //   So step 5 would catch that pair. But step 4: for the second '(' (at index1), the next bracket is the ')' at the end -> so step 4 catches it.
    //   Therefore, step 4 is sufficient? 
    //   However, what if the string is ")(": 
    //        For the '(' at index1: the next bracket? none -> skip.
    //        For the ')' at index0: step 5: previous bracket? none -> skip.
    //        So no non-emoticon pair, which is correct.
    //
    //   But what if we have "())": 
    //        brackets: [0:'(', 1:')', 2:')']
    //        For index0: next bracket is index1 -> which is ')', and substring s[1:1] is empty -> valid -> mark 0 and 1.
    //        For index1: step5: previous bracket is index0 -> which is '(', and substring empty -> valid -> mark 0 and 1 again.
    //        So it's redundant but safe.
    //
    //   Since n is 10^5, and the number of brackets is at most 10^5, doing two passes (one for '(' and one for ')') is O(n) per bracket? but the next bracket search: if we precompute the list of brackets, then for each bracket we can get the next/prev in O(1) by having the list of bracket indices.
    //
    // Implementation with list of brackets:
    //   vector<int> bracket_indices;
    //   for i=0 to n-1:
    //        if s[i]=='(' or s[i]==')', bracket_indices.push_back(i);
    //
    //   Then for each bracket index i in bracket_indices:
    //        if s[i]=='(':
    //            find the next bracket in bracket_indices: 
    //                pos = upper_bound(bracket_indices.begin(), bracket_indices.end(), i) 
    //                if pos != bracket_indices.end(), then j = *pos.
    //            if j exists and s[j]==')', check the substring s[i+1:j] for only letters and spaces.
    //        similarly for ')', but we can skip because step for '(' covers the pair.
    //
    //   However, to be safe and clear, we do only the opening bracket pass. Because the condition for closing bracket is symmetric and the pair will be found by the opening bracket.
    //
    //   But note: what if the next bracket for an opening bracket is '('? then skip. Only when it's ')' and the content condition holds, we mark.
    //
    //   So:
    //        non_emoticon_count = 0
    //        for each bracket index i in bracket_indices:
    //            if s[i]=='(':
    //                find next bracket index j (the first bracket after i)
    //                if j exists and s[j]==')':
    //                    check if for all k in [i+1, j-1]: s[k] is space or letter.
    //                    if yes, then mark i and j as non-emoticon (in a boolean array, but we don't need to store, we can count)
    //                    non_emoticon_count += 2;
    //
    //        However, caution: what if the same bracket is part of two pairs? 
    //            Example: "(()" -> 
    //                brackets: [0,1,2] where s[0]='(', s[1]='(', s[2]=')'
    //                For i=0: next bracket j=1 -> s[1]=='(', so skip.
    //                For i=1: next bracket j=2 -> s[2]==')', and substring s[2:2] is empty -> valid -> count +=2.
    //                So non_emoticon_count=2, which is correct.
    //
    //        Another: "())" -> 
    //                brackets: [0,1,2]
    //                i=0: j=1 -> s[1]==')', and substring s[1:1] is empty -> valid -> count +=2.
    //                i=1: j=2 -> s[2]==')', but next bracket for i=1 is j=2? and s[2]==')', so condition: for opening bracket at 1, the next bracket is ')' at 2 -> valid -> count +=2.
    //                total non_emoticon_count=4, but there are only 3 brackets! 
    //            This is a problem: the bracket at 1 is counted twice.
    //
    //        Why? Because the pair (0,1) and (1,2) both include bracket1. 
    //        But in "())", is bracket1 part of two non-emoticon pairs? 
    //            The problem: 
    //                For bracket1 (which is ')'): 
    //                    Condition for being non-emoticon: 
    //                        As a closing bracket: the nearest bracket to the left is '(' at 0 -> and between is empty -> valid -> so it is non-emoticon because of (0,1).
    //                    Also, for bracket1 as an opening bracket? but it's ')', so we don't consider it as opening.
    //                However, in our algorithm, we only consider opening brackets (so we look at i=0 and i=1, but i=1 is '('? no, in "())", bracket1 is ')', so in the list of brackets, we have:
    //                    bracket_indices: [0:'(', 1:')', 2:')']
    //                For i=0 (which is '('): next bracket is 1, which is ')', and empty between -> valid -> count +=2.
    //                For i=1: we skip because s[1]==')', so we only do the opening bracket pass for '('.
    //                Then for i=2: we skip because it's ')'.
    //                So count=2, which is correct (brackets 0 and 1 are non-emoticon, bracket2 is emoticon).
    //
    //        Therefore, we only iterate over the brackets that are '(' for the opening pass.
    //
    //   Revised plan:
    //        total_brackets = count of '(' and ')'
    //        non_emoticon_count = 0
    //        Precompute bracket_indices and also separate the opening bracket indices.
    //        vector<int> open_brackets;
    //        for i in [0, n):
    //            if s[i]=='(', open_brackets.push_back(i);
    //
    //        Then for each i in open_brackets:
    //            // find next bracket after i
    //            auto it = upper_bound(bracket_indices.begin(), bracket_indices.end(), i);
    //            if (it == bracket_indices.end()) continue;
    //            int j = *it;
    //            if (s[j] == ')') {
    //                // check substring s[i+1:j]
    //                bool valid = true;
    //                for (int k = i+1; k < j; k++) {
    //                    if (s[k] != ' ' && !isalpha(s[k])) {
    //                        valid = false;
    //                        break;
    //                    }
    //                }
    //                if (valid) {
    //                    non_emoticon_count += 2;
    //                }
    //            }
    //
    //        Then answer = total_brackets - non_emoticon_count;
    //
    //   Let's test with examples:
    //        Example 1: "Hi:)" 
    //            total_brackets = 1 (only ')')
    //            open_brackets: none -> non_emoticon_count=0
    //            answer = 1 - 0 = 1 -> but expected 3? no, wait the example output is 3 for a longer string.
    //        The example: "Hi:) (it is me) I have bad news:-(("
    //            We need to count all brackets and then subtract the non-emoticon ones.
    //            Brackets in the string:
    //                "Hi:)" -> 1 bracket: ')'
    //                " (it is me) " -> 2 brackets: '(' and ')'
    //                " I have bad news:-((" -> 3 brackets: '(', '(', '('? 
    //                    Actually: ":-((" -> two '(' at the end -> total brackets: 1+2+3=6? 
    //                But the example says output 3.
    //                So non-emoticon_count = 6 - 3 = 3.
    //            Which brackets are non-emoticon? 
    //                Only the pair in "(it is me)" -> 2 brackets -> non_emoticon_count=2? then answer=6-2=4, not 3.
    //            Let me write the string: "Hi:) (it is me) I have bad news:-(("
    //            Break down:
    //                H i : )   ( i t   i s   m e )   ...  : - ( ( 
    //                Brackets: 
    //                    index: 3 -> ')'
    //                    index: 5 -> '('
    //                    index: 14 -> ')'  (assuming " (it is me)" has 10 characters: space, '(', 'i','t',' ','i','s',' ','m','e',')' -> 9 characters? 
    //                Actually, let's count: 
    //                    "Hi:)" -> 4 chars: H,i,:,) -> bracket at 3.
    //                    then space -> 4
    //                    then '(' at 5
    //                    then "it is me": i,t, ,i,s, ,m,e -> 8 chars, then ')' at 5+1+8+1 = 15? 
    //                So brackets: 3,5,15, then later ":-((" -> 
    //                    after "news", we have ":-((" 
    //                    let's say the string length to "news" is 20, then ':' at 20, '-' at 21, '(' at 22, '(' at 23.
    //                Brackets: 3,5,15,22,23 -> 5 brackets? 
    //                But the example output is 3, so non-emoticon_count = 5-3=2? 
    //            The example says: "only the brackets that outline 'it is me' aren't emoticons", meaning 2 brackets are non-emoticon.
    //            So total brackets = 5, non-emoticon = 2, answer = 3.
    //            Therefore, we need to count 5 brackets.
    //
    //            Why did I think 6? 
    //                The string: "Hi:) (it is me) I have bad news:-((" 
    //                Let me write with positions (0-indexed):
    //                    H i : )   ( i t   i s   m e )   ... 
    //                    0 1 2 3 4 5 ...
    //                After "me)" there is a space? then "I have bad news:-(((" -> but the example says ":-((" (two '(').
    //                So brackets: 
    //                    pos3: ')'
    //                    pos5: '('
    //                    pos14: ')'  (if "it is me" is 8 letters + 2 spaces = 10, then 5+1+10 = 16? but the ')' is at 5+1+8+2 = 16? 
    //                Actually, let's count " (it is me)":
    //                    space (1) + '(' (1) + "it" (2) + space (1) + "is" (2) + space (1) + "me" (2) + ')' (1) = 1+1+2+1+2+1+2+1 = 11 characters.
    //                So if the string starts at 0:
    //                    "Hi:)" -> 0,1,2,3 -> then space at 4.
    //                    then '(' at 5, 
    //                    "it" -> 6,7; space->8; "is"->9,10; space->11; "me"->12,13; ')' at 14.
    //                Then " I have bad news:-((" 
    //                    space at 15, 
    //                    "I have bad news" -> 15+1 = 16, then "I" at 16, ... let's say "news" ends at 28 (13 letters? I have bad news: I (1) space (1) have (4) space (1) bad (3) space (1) news (4) = 1+1+4+1+3+1+4=15, so 16+15=31, then ':' at 31, '-' at 32, '(' at 33, '(' at 34.
    //                Brackets: 3,5,14,33,34 -> 5 brackets.
    //                non-emoticon_count = 2 (brackets 5 and 14) -> answer = 5-2=3.
    //
    //            So our algorithm: 
    //                open_brackets: [5,33,34]
    //                For i=5: next bracket after 5 is 14 (since brackets: 5,14,33,34). 
    //                    s[14]==')', and substring s[6:14] = "it is me" -> check: 
    //                         'i','t',' ','i','s',' ','m','e' -> all letters or spaces -> valid -> non_emoticon_count +=2 -> 2.
    //                For i=33: next bracket is 34 -> s[34]=='(', so skip.
    //                For i=34: next bracket? none -> skip.
    //                non_emoticon_count=2, total_brackets=5, answer=3. Correct.
    //
    //        Example 2: "((two plus two equals four))" 
    //            brackets: 
    //                s[0]='(', s[1]='(', ... s[n-2]=')', s[n-1]=')'
    //            open_brackets: [0,1]
    //            For i=0: next bracket is 1 -> s[1]=='(', skip.
    //            For i=1: next bracket is n-2 -> s[n-2]==')', and substring s[2:n-2] = "two plus two equals four", which is letters and spaces -> valid -> non_emoticon_count +=2.
    //            total_brackets=4, answer=4-2=2. Correct.
    //
    //   Edge: "()" 
    //        total_brackets=2
    //        open_brackets=[0]
    //        i=0: next bracket=1, s[1]==')', substring s[1:1] is empty -> valid -> non_emoticon_count=2.
    //        answer=0. Correct (because the problem says "()" is not part of emoticons, so count=0).
    //
    //   Edge: "()a" 
    //        brackets: [0,1] -> non_emoticon_count=2, answer=0.
    //        But the string has two brackets, both are non-emoticon, so 0 emoticon brackets. Correct.
    //
    //   Edge: "a()" 
    //        brackets: [1,2] -> open_brackets=[1]
    //        i=1: next bracket=2, s[2]==')', substring empty -> valid -> non_emoticon_count=2, answer=0.
    //
    //   Edge: "a)(b" 
    //        brackets: [1:')',2:'('] 
    //        open_brackets=[2]
    //        for i=2: next bracket? none -> non_emoticon_count=0, total_brackets=2, answer=2.
    //        Why? 
    //            The ')' at 1: no '(' to the left that is the nearest bracket? the nearest bracket to the left is none (because the only bracket to the left is none, since 1 is the first bracket) -> so it is emoticon.
    //            The '(' at 2: no bracket to the right -> so it is emoticon.
    //        So 2 emoticon brackets. Correct.
    //
    // Implementation note: 
    //   We need to get the next bracket for an opening bracket i. 
    //   We have the global list `bracket_indices` of all bracket positions (in increasing order). 
    //   We can precompute the list, then for each opening bracket index i, 
    //        use upper_bound to find the first element > i.
    //
    // Steps:
    //   1. Read string s.
    //   2. n = s.length()
    //   3. vector<int> bracket_indices;
    //      for i=0 to n-1: 
    //          if s[i]=='(' || s[i]==')', bracket_indices.push_back(i);
    //   4. int total_brackets = bracket_indices.size();
    //   5. vector<int> open_brackets;
    //      for each index in bracket_indices: 
    //          if s[index]=='(', push to open_brackets.
    //   6. int non_emoticon = 0;
    //      for each i in open_brackets:
    //          auto it = upper_bound(bracket_indices.begin(), bracket_indices.end(), i);
    //          if (it == bracket_indices.end()) continue;
    //          int j = *it;
    //          if (s[j] == ')') {
    //              bool valid = true;
    //              for (int k = i+1; k < j; k++) {
    //                  if (s[k] != ' ' && !isalpha((unsigned char)s[k])) {
    //                      valid = false;
    //                      break;
    //                  }
    //              }
    //              if (valid) {
    //                  non_emoticon += 2;
    //              }
    //          }
    //   7. cout << total_brackets - non_emoticon << endl;
    //
    // Note: isalpha might be affected by locale, but the problem says Latin letters, and the string is in English. 
    //   We can use isalpha from <cctype> with casting to unsigned char to avoid UB.
    //
    // Let's code accordingly.

    // However, note: the problem says the string may contain punctuation: "-", ":", ",", ";".
    //   So we must check only for spaces and letters (a-z, A-Z).

    // We'll write a helper lambda or just use:
    //   if (s[k] != ' ' && (s[k] < 'A' || (s[k] > 'Z' && s[k] < 'a') || s[k] > 'z'))
    //   but easier: 
    //        if (s[k] != ' ' && !((s[k]>='A' && s[k]<='Z') || (s[k]>='a' && s[k]<='z')))
    //
    // But to be safe and standard, use isalpha with cast.

    // Since the constraints are 10^5, and the inner loop might be long, but note: the total work over all opening brackets is O(n) because the bracket_indices list is not too long and the total length of all intervals we check is at most n.
    //   However, worst-case: many small intervals, e.g., "a(b)c(d)e(f)..." -> the intervals are short.
    //   But worst-case: one big interval: the entire string between two brackets -> O(n) for one opening bracket, and there could be O(n) opening brackets? 
    //        But the number of opening brackets is at most n/2, and the total length of all intervals we check is the sum of (j-i-1) for each valid pair? 
    //        But we break early if we find an invalid char.
    //   Worst-case: all intervals are invalid quickly (like if there's a punctuation at the beginning), so O(1) per bracket.
    //   Or if the intervals are long but we have to check them, worst-case total O(n^2). 
    //        Example: string of n/2 opening brackets followed by n/2 letters: "(((...)))" but with no closing brackets until the end? 
    //        Actually, the next bracket for the first '(' is the second '(', so we skip. For the second '(', next is third '(', skip, ... until the last '(', then next is the first ')', and then we check the entire string of letters -> O(n) for that one.
    //        Total work: O(n) because only one long check.
    //        In worst-case, the number of long checks is the number of opening brackets that have a matching closing bracket as the next bracket, which is at most the number of closing brackets, so O(n) total.
    //
    //   Why? Because each character in the string is checked at most once: 
    //        Consider a character at position k: it is checked only for the opening bracket that is the immediate left bracket before k (if that opening bracket has the next bracket as a closing bracket after k). 
    //        But actually, for a given opening bracket i, we check from i+1 to j-1. 
    //        And if two opening brackets i1 and i2 (i1<i2) both have the same next bracket j (which is a closing bracket), then the interval [i1+1, j-1] includes [i2+1, j-1]. 
    //        So the same characters might be checked multiple times.
    //
    //   Worst-case: 
    //        s = "((...))" with k opening brackets and then a letter and then k closing brackets? 
    //        Actually, the next bracket for the first opening bracket is the second opening bracket -> skip.
    //        for the second: next is third -> skip.
    //        ... 
    //        for the last opening bracket: next is the first closing bracket -> then we check the entire middle part, which might be long.
    //        Then the closing bracket after that: for the next opening bracket? none.
    //        So only one long check.
    //
    //   Another worst-case: 
    //        s = "()()()...()" (n/2 pairs)
    //        For each opening bracket i (at even indices), the next bracket is i+1 (a closing bracket), and the substring between is empty -> valid.
    //        So for each opening bracket, we do a check of length 0 -> O(1) per bracket.
    //        Total O(n).
    //
    //   Another: 
    //        s = "(a)(b)(c)..."
    //        For the first '(': next bracket is ')' at index1? no, the next bracket after 0 is 2? 
    //            brackets: 0,2,4,6,...
    //            For i=0: next bracket=2 -> s[2]==')', and substring s[1:2] = "a" -> check 1 char -> valid.
    //            For i=2: next bracket=4 -> s[4]==')', check s[3]='(' -> invalid? because '(' is not space or letter -> so skip.
    //        So only the first pair is valid.
    //        Total checks: 1 (for first pair) + 1 (for second pair, which breaks at s[3]) -> O(n).
    //
    //   Therefore, worst-case O(n^2) might not happen, but to be safe, note that n<=10^5, and worst-case O(n^2) is 10^10 which is too slow.
    //
    //   How to avoid O(n^2)? 
    //        We can precompute the next bracket, and then for the interval check, we can break early.
    //        But worst-case: a string like "((((...))))" with k opening brackets, then k-1 letters, then k closing brackets? 
    //            Actually, the next bracket for the first opening bracket is the second opening bracket -> skip.
    //            ... until the last opening bracket: then we check the entire block of letters (length = k-1) -> O(k).
    //            And k can be 50000, so 50000 per check, and there is only one such check -> 50000, which is acceptable for n=10^5.
    //
    //        The worst-case total work is the sum of the lengths of the intervals for which the next bracket is a closing bracket and the content is only spaces/letters. 
    //        But note: if the content is valid, then we count it, but the length of that interval is the number of characters between. 
    //        The sum of all such interval lengths over the entire string cannot exceed n, because the intervals are disjoint? 
    //            Not necessarily: consider "((a))": 
    //                brackets: 0,1,4,5
    //                For i=0: next bracket=1 -> skip.
    //                For i=1: next bracket=4 -> check s[2:4] = "a)" -> wait, no: s[4] is the closing bracket? 
    //                Actually, the string: 
    //                    index0: '(', 
    //                    index1: '(', 
    //                    index2: 'a', 
    //                    index3: ')', -> but wait, the next bracket after 1 is 3? 
    //                Let me write: 
    //                    s = "((a))" -> 
    //                    brackets: 0,1,3,4.
    //                For i=0: next=1 -> skip.
    //                For i=1: next=3 -> s[3]==')', and substring s[2:3] = "a" -> valid -> count 2.
    //                Then for i=0: we don't recheck, but the next bracket for 0 is 1, so we skip.
    //                The interval [2,3) is length 1.
    //                Now, if we have "((a)(b))", then:
    //                    brackets: 0,1,3,5,6,7
    //                    For i=0: next=1 -> skip.
    //                    For i=1: next=3 -> s[3]==')', but substring s[2:3]="a" -> valid -> count 2.
    //                    For i=3: next=5 -> s[5]==')'? no, s[5] is '('? 
    //                    Actually, the string: 
    //                        0:(, 1:(, 2:a, 3:), 4:(, 5:b, 6:), 7:)
    //                    brackets: 0,1,3,4,6,7.
    //                    For i=0: next=1 -> skip.
    //                    For i=1: next=3 -> valid -> count 2.
    //                    For i=3: next=4 -> s[4]=='(', skip.
    //                    For i=4: next=6 -> s[6]==')', substring s[5:6]="b" -> valid -> count 2.
    //                    Total non_emoticon=4, total_brackets=6, answer=2.
    //                The intervals checked: 
    //                    [2,3): "a" -> length=1
    //                    [5,6): "b" -> length=1
    //                Total work=2.
    //
    //        So the total work is the sum of the lengths of the valid intervals, and also the invalid intervals we break early.
    //        In the worst-case where all intervals are invalid, we break at the first character of the interval, so O(number of opening brackets that have a next bracket).
    //        And the number of such opening brackets is at most the number of brackets, which is <= n.
    //        So worst-case O(n).
    //
    //   Therefore, we can do the straightforward method.

    // Let's code accordingly.

    // Note: We must be cautious with the isalpha: use (unsigned char) cast.

    #include <cctype>
    #include <vector>
    #include <algorithm>
    #include <cctype>
    #include <iostream>
    #include <string>

    using namespace std;

    int main() {
        string s;
        getline(cin, s);
        int n = s.length();

        // Collect all bracket positions
        vector<int> bracket_indices;
        for (int i = 0; i < n; i++) {
            if (s[i] == '(' || s[i] == ')') {
                bracket_indices.push_back(i);
            }
        }

        int total_brackets = bracket_indices.size();
        vector<int> open_brackets;
        for (int idx : bracket_indices) {
            if (s[idx] == '(') {
                open_brackets.push_back(idx);
            }
        }

        int non_emoticon = 0;

        // For each opening bracket, find the next bracket
        for (int i : open_brackets) {
            // Find the first bracket index > i
            auto it = upper_bound(bracket_indices.begin(), bracket_indices.end(), i);
            if (it == bracket_indices.end()) {
                continue;
            }
            int j = *it;
            if (s[j] == ')') {
                // Check substring s[i+1 ... j-1]
                bool valid = true;
                for (int k = i+1; k < j; k++) {
                    if (s[k] != ' ' && !isalpha((unsigned char)s[k])) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    non_emoticon += 2;
                }
            }
        }

        cout << total_brackets - non_emoticon << endl;

        return 0;
    }
