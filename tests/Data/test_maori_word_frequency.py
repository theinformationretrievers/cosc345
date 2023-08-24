import filecmp
import subprocess

def test_maori_word_frequency():
    expected_output = "./tests/Data/test_io_data/expected_maori_frequencies.json"
    generated_output = "./tests/Data/test_io_data/generated_maori_frequencies.json"
    
    inputfile = "./src/Data/raw_data/maori_word_frequency.docx"
    
    subprocess.run(["python3", "src/Data/scripts/maori_word_frequency.py", "--filepath", generated_output])
    assert filecmp.cmp(expected_output, generated_output)