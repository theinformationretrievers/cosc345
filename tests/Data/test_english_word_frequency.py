import filecmp
import subprocess

def test_english_word_frequency():
    expected_output = "./tests/Data/test_io_data/expected_english_frequencies.json"
    generated_output = "./tests/Data/test_io_data/generated_english_frequencies.json"
    
    inputfile = "./src/Data/raw_data/unigram_freq.csv"
    
    subprocess.run(["python3", "src/Data/scripts/english_word_frequency.py", "--filepath", generated_output, "--input", inputfile])
    assert filecmp.cmp(expected_output, generated_output)