import argparse
import os
import cv2
import numpy as np


def get_args():
    parser = argparse.ArgumentParser(description='Process args for splitting red-blue stereo image to left and right images')
    parser.add_argument('-i', '--input', required=True, action='store', help='File path for Red-Blue Stereo Image')
    parser.add_argument('-o', '--output', required=False, action='store', help='Folder path for left and right images to be stored')
    parser.add_argument('-c', '--cyan', required=False, action='store_true', help='Input image is Red-Cyan image')
    args = parser.parse_args()
    return args


def split_image(anaglyph_image_file_path, output_folder_path=None, red_cyan=False):
    if not os.path.exists(anaglyph_image_file_path):
        print(f"The file: {anaglyph_image_file_path} doesn't exist")
        return
    if output_folder_path:
        if not os.path.exists(output_folder_path):
            print(f"The folder: {output_folder_path} doesn't exist")
            return
        elif not os.path.isdir(output_folder_path):
            print(f"Not a folder: {output_folder_path}")
            return
    else:
        output_folder_path = os.path.dirname(anaglyph_image_file_path)
    # Load the anaglyph image
    img = cv2.imread(anaglyph_image_file_path)
    # Split into color channels
    b, g, r = cv2.split(img)
    # Create full alpha channel (255 = fully opaque)
    alpha = np.full_like(r, 255)
    # Create left image (from red channel)
    left = cv2.merge([r, r, r, alpha])  # grayscale red into RGB for visibility
    # Create right image (from blue and/or green channel)
    if red_cyan:
        # Option 1 (cyan): use both green and blue
        right = cv2.merge([b, g, (b + g) // 2, alpha])  # optional blend
    else:
        # Option 2: just blue
        right = cv2.merge([b, b, b, alpha])
    # Save or display the images
    file_name_without_extension = os.path.splitext(os.path.basename(anaglyph_image_file_path))[0]
    cv2.imwrite(output_folder_path + os.sep + file_name_without_extension + '.left.png', left)
    cv2.imwrite(output_folder_path + os.sep + file_name_without_extension + '.right.png', right)


def main():
    args = get_args()
    split_image(args.input, args.output, args.cyan)


if __name__ == "__main__":
    main()















