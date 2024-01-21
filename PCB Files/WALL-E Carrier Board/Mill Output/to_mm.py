# Converts the output of pcb2gcode to units of millimeters because chilipeppr only supports millimeters for
# autoleveling. Also removes G04 commands because G04 P0 breaks tinyg. 

import re

def convert_gcode(input_file, output_file):
    # Define the conversion factor from inches to millimeters
    conversion_factor = 25.4

    # Open the input file in read mode
    with open(input_file, 'r') as input_file:
        # Read the lines from the input file
        lines = input_file.readlines()

    # Open the output file in write mode
    with open(output_file, 'w') as output_file:
        # Iterate through each line in the input file
        for line in lines:
            # Check if the line contains a G-code command
            if line.startswith(('G0', 'G1', 'G2', 'G3')):
                # Extract X, Y, Z, and F coordinates using regular expressions
                x_match = re.search(r'X(-?\d+\.?\d*)', line)
                y_match = re.search(r'Y(-?\d+\.?\d*)', line)
                z_match = re.search(r'Z(-?\d+\.?\d*)', line)
                f_match = re.search(r'F(\d+\.?\d*)', line)

                # Convert the coordinates if present
                if x_match:
                    x_value = float(x_match.group(1)) * conversion_factor
                    line = re.sub(r'X(-?\d+\.?\d*)', f'X{x_value:.4f}', line)
                if y_match:
                    y_value = float(y_match.group(1)) * conversion_factor
                    line = re.sub(r'Y(-?\d+\.?\d*)', f'Y{y_value:.4f}', line)
                if z_match:
                    z_value = float(z_match.group(1)) * conversion_factor
                    line = re.sub(r'Z(-?\d+\.?\d*)', f'Z{z_value:.4f}', line)
                if f_match:
                    f_value = float(f_match.group(1)) * conversion_factor
                    line = re.sub(r'F(\d+\.?\d*)', f'F{f_value:.4f}', line)

            # Update the G-code command for unit mode (G20 to G21)
            line = line.replace('G20', 'G21')
            line = line.replace('Units == INCHES.', 'Units == MM.')

            # Remove lines starting with G04
            if line.startswith('G04'):
                continue

            # Write the modified line to the output file
            output_file.write(line)

if __name__ == '__main__':
    
    # Example usage
    input_file_paths = ['drill.ngc', 'front.ngc', 'back.ngc', 'outline.ngc', 'milldrill.ngc']
    for input_file_path in input_file_paths:
        output_file_path = input_file_path.replace('.ngc', '_mm.ngc')
        convert_gcode(input_file_path, output_file_path)
