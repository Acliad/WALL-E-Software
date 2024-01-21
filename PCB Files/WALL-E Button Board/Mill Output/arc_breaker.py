from pathlib import Path
import re
import math

I_REGEX = re.compile(r'I(-?\d+\.?\d*)')
J_REGEX = re.compile(r'J(-?\d+\.?\d*)')
X_REGEX = re.compile(r'X(-?\d+\.?\d*)')
Y_REGEX = re.compile(r'Y(-?\d+\.?\d*)')
Z_REGEX = re.compile(r'Z(-?\d+\.?\d*)')


def arc_breaker(input: str | Path, output) -> None:
    current_z = 0
    current_x = 0
    current_y = 0

    with open(input, 'r') as gcode:
        gcode_in = gcode.readlines()
    with open(output, 'w') as gcode_out:
        gcode_out.write('(Processed with ArcBreaker)\n')
        for line in gcode_in:
            if re.match(r'G0?2\b', line):
                # This is an arc We need to convert the arc's trajectory to a series of straight lines. To make thinking
                # about easier, we'll calculate the movement at 0,0, and then translate it to the current position.

                # Get the arc start values I and J values in our 0, 0 coordinate system
                i_start = -float(I_REGEX.search(line).group(1))
                j_start = -float(J_REGEX.search(line).group(1))
                # get the arc's end point in our 0, 0 coordinate system
                i_end = current_x-float(X_REGEX.search(line).group(1))
                j_end = current_y-float(Y_REGEX.search(line).group(1))

                radius = (i_start**2 + j_start**2)**0.5
                # Calculate the angle of the arc
                theta = math.atan2(j_end, i_end) - math.atan2(j_start, i_start)
                theta = theta if theta > 0 else theta + 2*math.pi
                # Calculate the length of the arc
                arc_length = radius * theta
                # Calculate the number of segments we'll need to approximate the arc
                num_segments = math.ceil(arc_length/0.05)

                z_match = Z_REGEX.search(line)
                if z_match:
                    # This arc has a z movement, we need to calculate the z movement per segment
                    z_dist = float(z_match.group(1)) - current_z
                else:
                    # This arc doesn't have a z movement, we'll just assume it's the same as the previous line
                    z_dist = 0

                for segment in range(num_segments + 1):
                    segment_z = current_z + z_dist * segment/num_segments
                    # Calculate the angle of the current segment (lockwise rotation for G02)
                    segment_angle = theta * (num_segments - segment)/num_segments
                    # Calculate the x and y coordinates of the current segment
                    segment_x = radius * math.cos(segment_angle)
                    segment_y = radius * math.sin(segment_angle)
                    # Translate the segment to the current position, acounting for the arc's start point
                    segment_x += current_x - i_start
                    segment_y += current_y - j_start
                    # Write the segment to the output file
                    gcode_out.write(f'G1 X{segment_x:.3f} Y{segment_y:.3f} Z{segment_z:.3f}\n')
            else:
                # This is a straight line, just write it to the output file
                gcode_out.write(line)

            # Update the current position
            if re.search(r'X', line):
                current_x = float(X_REGEX.search(line).group(1))
            if re.search(r'Y', line):
                current_y = float(Y_REGEX.search(line).group(1))
            if re.search(r'Z', line):
                current_z = float(Z_REGEX.search(line).group(1))

input_file = Path(r'/Users/isaacrex/Library/Mobile Documents/com~apple~CloudDocs/Projects/Wall-E/PCB Files/WALL-E Button Board/Mill Output/milldrill_mm.ngc')
ouput_file = Path(r'/Users/isaacrex/Library/Mobile Documents/com~apple~CloudDocs/Projects/Wall-E/PCB Files/WALL-E Button Board/Mill Output/milldrill_mm_arcbreaker.ngc')
arc_breaker(input_file, ouput_file)