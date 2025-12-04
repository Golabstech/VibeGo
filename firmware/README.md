# OTA Firmware Update

To release a new version via OTA:

1. Build the project in PlatformIO (`pio run`).
2. Locate the compiled binary at `.pio/build/esp32s3-jc4827w543/firmware.bin`.
3. Copy `firmware.bin` into this folder (`firmware/`).
4. Commit and push to GitHub:

    ```bash
    git add firmware/firmware.bin
    git commit -m "Release version X.Y.Z"
    git push
    ```

5. The devices will detect the change and update automatically on next boot.
