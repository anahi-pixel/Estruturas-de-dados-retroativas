import os
import subprocess
import filecmp

# Directories
input_dir = 'inputs/'
expected_output_dir = 'expected_outputs/'
actual_output_dir = 'actual_outputs/'

# Ensure the actual output directory exists
os.makedirs(actual_output_dir, exist_ok=True)

# List all input files
input_files = [f for f in os.listdir(input_dir) if f.endswith('.txt')]

# Loop through each input file
for input_file in input_files:
    input_path = os.path.join(input_dir, input_file)
    expected_output_path = os.path.join(expected_output_dir, input_file)
    actual_output_path = os.path.join(actual_output_dir, input_file)

    print(input_path)

    # Run your C++ program with the input file and write the output to the actual output file
    with open(actual_output_path, 'w') as actual_output_file:
        subprocess.run(['./main', input_path], stdout=actual_output_file)

    # Compare the actual output with the expected output
    if filecmp.cmp(expected_output_path, actual_output_path, shallow=False):
        print(f'Test {input_file} passed.')
    else:
        print(f'Test {input_file} failed.')
