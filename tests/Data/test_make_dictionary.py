# test_make_dictionary.py
import filecmp
import subprocess


def test_make_dict_pos():
    expected_output = "./tests/Data/test_io_data/expected_output_dictionary_pos.json"
    generated_output = "./tests/Data/test_io_data/generated_output_dictionary_pos.json"

    print("running file.. please wait.")
    subprocess.run(["python3", "./src/Data/scripts/make_dictionary.py",
                   "--filepath", generated_output, "--option", "pos"])
    assert filecmp.cmp(expected_output, generated_output)


def test_make_dict_no_pos():
    expected_output = "./tests/Data/test_io_data/expected_output_dictionary_no_pos.json"
    generated_output = "./tests/Data/test_io_data/generated_output_dictionary_no_pos.json"

    print("running file.. please wait.")
    subprocess.run(["python3", "./src/Data/scripts/make_dictionary.py",
                   "--filepath", generated_output, "--option", "no_pos"])
    assert filecmp.cmp(expected_output, generated_output)
