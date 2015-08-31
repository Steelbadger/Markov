#pragma once
#include <string>
#include <map>
#include <random>
#include <vector>
#include <regex>


class Markov
{
public:
	void SetOrder(int ord) {
		order = ord;
	}

	void ParseText(std::string input) {
		// Define what constitutes a word. Punctuation, in this case, counts.
		std::regex word("([\\w-']+|[,\\.!\\?][\"']|['\"]|[\\.,:;!\\?\\n])");
		std::smatch results;

		// Find all matches
		auto entries_begin = std::sregex_iterator(input.begin(), input.end(), word);
		auto entries_end = std::sregex_iterator();

		// Track the preceeding words in a vector
		std::vector<std::string> prev_words;
		prev_words.resize(order, "\n");

		
		std::string next_word;
		std::string first;

		// For every match
		for (auto it = entries_begin; it != entries_end; ++it) {
			// Push back on the vector of preceeding words (old words fall off the end)
			// Store the state of the vector as a concat-ed string
			first = prev_words[0];
			for (int i = 1; i < order; ++i) {
				first.append(prev_words[i]);
				prev_words[i - 1] = prev_words[i];
			}
			// Store the match to a string
			next_word = (*it)[1];
			// Convert to lowercase
			std::transform(next_word.begin(), next_word.end(), next_word.begin(), ::tolower);
			// Store the new word in the preceeding words vector for use next time round
			prev_words[order - 1] = next_word;
			// Store the relationship
			RegisterChain(first, next_word);
		}
	}

	std::string GetMarkovString(int words_approx) {
		std::string output;
		std::string next;
		std::string last;
		std::vector<std::string> last_strings;
		last_strings.resize(order, "\n");
		// While we still want more words (or are not at a convenient stopping point)
		while (words_approx > 0 || next != "\n") {
			// Get the preceeding word concat-ed string and push back on the vector of words
			last = last_strings[0];
			for (int i = 1; i < order; ++i) {
				last.append(last_strings[i]);
				last_strings[i - 1] = last_strings[i];
			}
			// Use the preceeding words to find the next word
			next = GetNextWord(last);
			// Push that new word to the vector of preceeding words for next time around
			last_strings[order - 1] = next;

			// Do a little housekeeping to keep things tidier, add spaces when words are real words
			if (next != "," && next != "." && next != ",\"" && next != ".\"" && next != ",'" && next != ".'") {
				output.append(" ");
				words_approx--;
			}

			// Store the word to the output string
			output.append(next);
		}
		return output;
	}

	std::string GetNextWord(std::string first) {
		// First fetch the map of next-word options from the primary map
		auto options = markov_map.at(first);
		std::uniform_int_distribution<int> dist(0, markov_count[first]);
		// Generate a random number (up to max of total number of times this string occurred)
		int val = dist(generator);
		auto it = options.begin();
		// Iterate over options and subtract from generated number. Simple probability method.
		do {
			val -= it->second;
			if (val > 0) ++it;
		} while (val > 0);
		// Return the value
		return it->first;
	}

	void SeedGenerator(int seed) {
		generator.seed(seed);
	}

	void RegisterChain(std::string first, std::string last) {
		markov_map[first][last]++;
		markov_count[first]++;
	}

private:
	std::map<std::string, std::map<std::string, int>> markov_map;
	std::map<std::string, int> markov_count;
	std::default_random_engine generator;
	int order;
};