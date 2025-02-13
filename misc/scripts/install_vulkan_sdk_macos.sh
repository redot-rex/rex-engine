#!/usr/bin/env sh
# This script checks for the latest Vulkan SDK version,
# downloads it if a newer version is available, and then installs it for Redot.
# It requires 'jq' to parse JSON responses.

set -euo pipefail
IFS=$'\n\t'

# Initialize the variable to hold the latest version string.
latest_version_str=""

# If 'jq' is installed, fetch the current Vulkan SDK release info.
if command -v jq >/dev/null 2>&1; then
    curl -L "https://sdk.lunarg.com/sdk/download/latest/mac/config.json" -o /tmp/vk-sdk-config.json
    latest_version_str=$(jq -r '.version' /tmp/vk-sdk-config.json)
    # Convert version numbers (x.y.z.w) into a comparable numeric format.
    latest_version_num=$(echo "$latest_version_str" | awk -F. '{ printf("%d%02d%04d%02d", $1, $2, $3, $4); }')
    rm -f /tmp/vk-sdk-config.json

    # Loop through installed Vulkan SDK directories to verify if an update is needed.
    for sdk_dir in "$HOME/VulkanSDK"/*; do
        if [ -d "$sdk_dir" ]; then
            installed_version_num=$(basename "$sdk_dir" | awk -F. '{ printf("%d%02d%04d%02d", $1, $2, $3, $4); }')
            if [ "$installed_version_num" -ge "$latest_version_num" ]; then
                echo "A current or newer Vulkan SDK is already installed. Update skipped."
                exit 0
            fi
        fi
    done
fi

# Download and extract the latest Vulkan SDK package.
curl -L "https://sdk.lunarg.com/sdk/download/latest/mac/vulkan-sdk.zip" -o /tmp/vk-sdk.zip
unzip /tmp/vk-sdk.zip -d /tmp

# Run the appropriate installer based on the extracted directory structure.
if [ -d "/tmp/InstallVulkan-$latest_version_str.app" ]; then
    "/tmp/InstallVulkan-$latest_version_str.app/Contents/MacOS/InstallVulkan-$latest_version_str" \
        --accept-licenses --default-answer --confirm-command install
    rm -rf "/tmp/InstallVulkan-$latest_version_str.app"
elif [ -d "/tmp/InstallVulkan.app" ]; then
    "/tmp/InstallVulkan.app/Contents/MacOS/InstallVulkan" \
        --accept-licenses --default-answer --confirm-command install
    rm -rf "/tmp/InstallVulkan.app"
fi

rm -f /tmp/vk-sdk.zip

echo "Vulkan SDK installation complete! You can now compile Redot by running 'scons'."
