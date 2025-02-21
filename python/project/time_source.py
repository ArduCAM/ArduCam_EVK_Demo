import argparse
import re
import numpy as np
import cv2

from typing import Optional, cast
from ArducamEvkSDK import Camera, Param, LoggerLevel, Frame, TimeSource
from img_cvt_utils import show_image


class R:
    open_error = "open camera error! ret={}"
    time_source_error = "Failed to set time source. ret={}"
    get_config = "config.width={}, config.height={}"
    get_frame = "get frame({}x{}) from camera."
    invalid_color = "Invalid color format."
    invalid_color_length = "Invalid color length."


def log_callback(level, msg):
    print(msg)


def hex_to_rgb(hex_color):
    hex_color = hex_color.lstrip("#")
    return tuple(reversed(tuple(int(hex_color[i : i + 2], 16) for i in (0, 2, 4))))


def draw_diff_times(
    img,
    diff_times,
    diff_times_size,
    /,
    thickness=2,
    color="ff0000",
    background="000000",
    resolution=(640, 480),
    y_segments=10,
):
    w, h = resolution
    margin_left = 50  # Space for y-axis labels
    margin_top = 20  # Space for top margin
    margin_bottom = 20  # Space for bottom margin

    if not diff_times:
        return img

    # Calculate the scaling factors
    max_diff = max(diff_times)
    min_diff = min(diff_times)
    y_range = max_diff - min_diff
    # Enlarge the y_range by 20% to make the graph look better
    min_diff = min_diff - y_range * 0.1
    max_diff = max_diff + y_range * 0.1
    y_range = max_diff - min_diff

    draw = getattr(draw_diff_times, "draw", 0)
    refresh = getattr(draw_diff_times, "refresh", 0)
    if refresh == 1:
        draw_diff_times.refresh = 0
        draw = 1
    else:
        min_diff = min(getattr(draw_diff_times, "min_diff", max_diff), min_diff)
        max_diff = max(getattr(draw_diff_times, "max_diff", min_diff), max_diff)
        y_range = max_diff - min_diff
    draw_diff_times.min_diff = min_diff
    draw_diff_times.max_diff = max_diff

    y_scale = (h - margin_bottom) / y_range if y_range != 0 else 1
    x_scale = (w - margin_left) / diff_times_size
    x_axis_step = 1 if diff_times_size <= 10 else diff_times_size // 10
    scatter_mode = diff_times_size > 200
    x_step = 1
    x_step_offset = x_step

    # Draw the axes
    axis_color = (255, 255, 255)
    axis_thickness = 1
    line_color = hex_to_rgb(color)

    if draw:
        # Create a blank canvas
        canvas = np.zeros((h, w, 3), dtype=np.uint8)
        bg_color = hex_to_rgb(background)
        canvas[:] = bg_color

        cv2.line(
            canvas,
            (margin_left, h - margin_bottom),
            (w, h - margin_bottom),
            axis_color,
            axis_thickness,
        )
        cv2.line(
            canvas,
            (margin_left, margin_top),
            (margin_left, h - margin_bottom),
            axis_color,
            axis_thickness,
        )

        # Draw the reference lines for x-axis
        for i in range(x_axis_step, diff_times_size, x_axis_step):
            x = int(margin_left + i * x_scale)
            cv2.line(
                canvas,
                (x, margin_top),
                (x, h - margin_bottom),
                axis_color,
                axis_thickness,
                lineType=cv2.LINE_AA,
            )

        # Draw the reference lines and y-axis labels
        for i in range(1, y_segments + 1):
            y = int(i * (h - margin_bottom) / y_segments)
            cv2.line(
                canvas,
                (margin_left, y),
                (w, y),
                axis_color,
                axis_thickness,
                lineType=cv2.LINE_AA,
            )
            label = f"{min_diff + (y_segments - i) * y_range / y_segments:.1f}"
            cv2.putText(
                canvas,
                label,
                (5, y),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.5,
                axis_color,
                1,
                cv2.LINE_AA,
            )

        # Draw the diff times
        if scatter_mode:
            for i in range(x_step_offset, len(diff_times), x_step):
                x = int(margin_left + i * x_scale)
                if x < margin_left:
                    continue
                y = int(h - margin_bottom - (diff_times[i] - min_diff) * y_scale)
                cv2.circle(canvas, (x, y), thickness, line_color, -1)
        else:
            for i in range(x_step_offset, len(diff_times), x_step):
                x1 = int(margin_left + (i - x_step) * x_scale)
                if x1 < margin_left:
                    continue
                y1 = int(
                    h - margin_bottom - (diff_times[i - x_step] - min_diff) * y_scale
                )
                x2 = int(margin_left + i * x_scale)
                y2 = int(h - margin_bottom - (diff_times[i] - min_diff) * y_scale)
                cv2.line(canvas, (x1, y1), (x2, y2), line_color, thickness)
    else:
        canvas = img
        if canvas is None:
            raise ValueError("canvas is None")

        # only draw the last line
        last = len(diff_times) - 1
        if scatter_mode:
            x = int(margin_left + (last) * x_scale)
            y = int(h - margin_bottom - (diff_times[-1] - min_diff) * y_scale)
            cv2.circle(canvas, (x, y), thickness, line_color, -1)
        else:
            x1 = int(margin_left + (last - x_step) * x_scale)
            y1 = int(h - margin_bottom - (diff_times[-1 - x_step] - min_diff) * y_scale)
            x2 = int(margin_left + (last) * x_scale)
            y2 = int(h - margin_bottom - (diff_times[-1] - min_diff) * y_scale)
            cv2.line(canvas, (x1, y1), (x2, y2), line_color, thickness)

    return canvas


def main(config, time_source, diff_times_size=50, color="ff0000"):
    camera = Camera()
    param = Param()
    param.config_file_name = config
    param.bin_config = config.endswith(".bin")
    r = camera.open(param)
    if not r:
        raise Exception(R.open_error.format(camera.last_error_message))
    camera.set_message_callback(log_callback)
    camera.log_level = LoggerLevel.Info
    print(camera.usb_type)

    # if r != 0:
    #     raise Exception(Messages.open_error.format(r))
    camera.init()
    if not camera.set_time_source(time_source):
        raise Exception(R.time_source_error.format(camera.last_error_message))

    camera.start()
    config = camera.config
    print(R.get_config.format(config.width, config.height))

    skip_frame = 10
    begin_time = 0
    diff_times = []
    draw_diff_times.draw = 1
    img = None

    while True:
        image = cast(Optional[Frame], camera.capture(1000))
        if image is None:
            continue
        show_image(image)
        # print(Messages.get_frame.format(image.format.width, image.format.height))
        key = cv2.waitKey(1)
        if key == ord("q"):
            break
        elif key == ord("s"):
            if time_source == TimeSource.System:
                new_time_source = TimeSource.Firmware
            else:
                new_time_source = TimeSource.System
            if not camera.set_time_source(new_time_source):
                print(R.time_source_error.format(camera.last_error_message))
                continue
            time_source = new_time_source
            skip_frame = 5
        elif key == ord("r"):
            draw_diff_times.refresh = 1

        if skip_frame > 0:
            skip_frame -= 1
            begin_time = image.timestamp
            continue
        diff = image.timestamp - begin_time
        begin_time = image.timestamp
        if time_source == TimeSource.Firmware:
            diff /= 10000
        diff_times.append(diff)
        if len(diff_times) >= diff_times_size:
            for i in range(diff_times_size * 3 // 4):
                diff_times.pop(0)
            draw_diff_times.draw = 1
            # draw_diff_times.x_offset = getattr(draw_diff_times, "x_offset", 0) + 1

        img = draw_diff_times(img, diff_times, diff_times_size, color=color)
        cv2.imshow("Diff Times", img)
    camera.stop()
    camera.close()


if __name__ == "__main__":
    parse = argparse.ArgumentParser(description="Arducam Evk SDK example.")
    # -c/--config <str>
    # -t/--time-source <str> # only support "0", "system" or "1", "firmware"
    # -s/--size <int>
    # -C/--color <str> # only support #rrggbb
    parse.add_argument(
        "-c",
        "--config",
        help="Path to config file.",
        type=str,
        required=True,
    )
    parse.add_argument(
        "-t",
        "--time-source",
        help="Source of time for frame capture.",
        type=str,
        required=False,
    )
    parse.add_argument(
        "-s",
        "--size",
        help="Size of the captured image.",
        type=int,
        required=False,
        default=50,
    )
    parse.add_argument(
        "-C",
        "--color",
        help="Color format for the image in #rrggbb.",
        type=str,
        required=False,
        default="#ff0000",  # Added default value for color
    )

    args = parse.parse_args()
    time_source = TimeSource.System
    if args.time_source:
        if args.time_source == "0" or args.time_source.lower() == "system":
            time_source = TimeSource.System
        elif args.time_source == "1" or args.time_source.lower() == "firmware":
            time_source = TimeSource.Firmware
    color = "#ff0000"
    if args.color:
        if not re.match(r"^#?[0-9a-fA-F]{6}$", args.color) and not re.match(
            r"^#?[0-9a-fA-F]{3}$", args.color
        ):
            raise ValueError(R.invalid_color)
        if len(args.color) == 3:
            color = "".join([c * 2 for c in args.color])
        elif len(args.color) == 4:
            color = "".join([c * 2 for c in args.color[1:]])
        elif len(args.color) == 6:
            color = args.color
        elif len(args.color) == 7:
            color = args.color[1:]
        else:
            raise ValueError(R.invalid_color_length)

    main(args.config, time_source, args.size, color)
