#!/usr/bin/env python3

# pyinstaller --onefile mise_env_sync.py

import sys
import winreg
import os
import ctypes
from ctypes import wintypes

class MiseEnvSync:
    ENV_KEY = r"Environment"

    def __init__(self):
        self.add_vars = {}
        self.add_paths = []
        self.whitelist_vars = {}
        self.whitelist_paths = []
        self.load_config_from_env()

    def load_config_from_env(self):
        """Load configuration from environment variables (set by mise task)."""
        add_vars = os.environ.get('MISE_ADD_VARS', '')
        add_paths = os.environ.get('MISE_ADD_PATHS', '')
        keep_vars = os.environ.get('MISE_KEEP_VARS', '')
        keep_paths = os.environ.get('MISE_KEEP_PATHS', '')

        if add_vars:
            self.add_vars = self._parse_vars(add_vars)

        if add_paths:
            self.add_paths = self._parse_paths(add_paths)

        if keep_vars:
            self.whitelist_vars = self._parse_vars(keep_vars)

        if keep_paths:
            self.whitelist_paths = self._parse_paths(keep_paths)

        if self.add_vars or self.add_paths:
            print(f"[OK] Loaded {len(self.add_vars)} variables and {len(self.add_paths)} paths to export")
        if self.whitelist_vars or self.whitelist_paths:
            print(f"[OK] Loaded {len(self.whitelist_vars)} whitelisted variables and {len(self.whitelist_paths)} whitelisted paths")

    def _parse_vars(self, vars_str: str) -> dict:
        """Parse variables string (supports ; or newline separated)."""
        result = {}
        # Try newline first (from TOML multiline string), then semicolon
        separators = ['\n', ';']
        items = []

        for sep in separators:
            if sep in vars_str:
                items = [item.strip() for item in vars_str.split(sep) if item.strip()]
                break
        else:
            items = [vars_str.strip()] if vars_str.strip() else []

        for item in items:
            if '=' in item:
                key, value = item.split('=', 1)
                result[key.strip()] = value.strip()
            elif item:
                result[item] = ''

        return result

    def _parse_paths(self, paths_str: str) -> list:
        """Parse path string (supports ; or newline separated)."""
        # Try newline first (from TOML multiline string), then semicolon
        separators = ['\n', ';']
        for sep in separators:
            if sep in paths_str:
                return [p.strip() for p in paths_str.split(sep) if p.strip()]
        return [paths_str.strip()] if paths_str.strip() else []

    def get_all_user_env_vars(self):
        vars = {}
        try:
            with winreg.OpenKey(winreg.HKEY_CURRENT_USER, self.ENV_KEY) as key:
                i = 0
                while True:
                    try:
                        name, value, _ = winreg.EnumValue(key, i)
                        vars[name] = value
                        i += 1
                    except OSError:
                        break
        except FileNotFoundError:
            pass
        return vars

    def set_user_env_value(self, name: str, value: str):
        if name.upper() == "PATH":
            value = value.replace("/", "\\")
            # Ensure PATH ends with semicolon to prevent concatenation issues
            if value and not value.endswith(";"):
                value += ";"

        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, self.ENV_KEY, 0, winreg.KEY_SET_VALUE) as key:
            winreg.SetValueEx(key, name, 0, winreg.REG_EXPAND_SZ, value)

    def delete_user_env_value(self, name: str):
        try:
            with winreg.OpenKey(winreg.HKEY_CURRENT_USER, self.ENV_KEY, 0, winreg.KEY_SET_VALUE) as key:
                winreg.DeleteValue(key, name)
        except FileNotFoundError:
            pass

    def broadcast_environment_change(self):
        """Broadcast WM_SETTINGCHANGE to notify all windows of environment variable changes."""
        HWND_BROADCAST = 0xFFFF
        WM_SETTINGCHANGE = 0x001A
        SMTO_ABORTIFHUNG = 0x0002

        result = ctypes.c_long()
        SendMessageTimeoutW = ctypes.windll.user32.SendMessageTimeoutW
        SendMessageTimeoutW.argtypes = [wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPCWSTR, wintypes.UINT, wintypes.UINT, ctypes.POINTER(ctypes.c_long)]
        SendMessageTimeoutW.restype = wintypes.LPARAM

        SendMessageTimeoutW(
            HWND_BROADCAST,
            WM_SETTINGCHANGE,
            0,
            "Environment",
            SMTO_ABORTIFHUNG,
            5000,
            ctypes.byref(result)
        )
        print("[OK] Broadcasted environment change notification")

    def normalize_path(self, path: str) -> str:
        expanded = os.path.expandvars(path)
        return expanded.replace("/", "\\").lower().rstrip("\\")

    def paths_match(self, path1: str, path2: str) -> bool:
        return self.normalize_path(path1) == self.normalize_path(path2)

    def merge_paths(self, *path_lists):
        seen = set()
        merged = []
        for path_list in path_lists:
            for p in path_list:
                p_normalized = self.normalize_path(p)
                if p_normalized not in seen:
                    seen.add(p_normalized)
                    merged.append(p)
        return merged

    def clean(self):
        all_vars = self.get_all_user_env_vars()

        if not self.whitelist_vars and not self.whitelist_paths:
            print("[WARNING] No whitelist configured (set MISE_KEEP_VARS or MISE_KEEP_PATHS)")
            print("  All user environment variables will be removed!")
            response = input("  Continue? (yes/no): ")
            if response.lower() != 'yes':
                print("[ABORTED]")
                return

        print(f"Cleaning user environment variables (preserving {len(self.whitelist_vars)} whitelisted)...")

        removed_count = 0
        preserved_count = 0
        path_was_deleted = False

        whitelist_upper_map = {k.upper(): k for k in self.whitelist_vars.keys()}

        for var_name, var_value in all_vars.items():
            is_path_whitelisted = (var_name.upper() == "PATH" and self.whitelist_paths)
            is_var_whitelisted = var_name.upper() in whitelist_upper_map

            if is_path_whitelisted or is_var_whitelisted:
                if var_name.upper() == "PATH" and self.whitelist_paths:
                    current_paths = [p.strip() for p in var_value.split(";") if p.strip()]
                    preserved_paths = []

                    for current_path in current_paths:
                        for whitelist_path in self.whitelist_paths:
                            if self.paths_match(current_path, whitelist_path):
                                preserved_paths.append(current_path)
                                break

                    if preserved_paths:
                        self.set_user_env_value("PATH", ";".join(preserved_paths))
                        print(f"  [OK] Preserved PATH ({len(preserved_paths)} whitelisted entries)")
                        preserved_count += 1
                    else:
                        self.delete_user_env_value(var_name)
                        path_was_deleted = True
                        removed_count += 1
                else:
                    self.delete_user_env_value(var_name)
                    correct_name = whitelist_upper_map[var_name.upper()]
                    self.set_user_env_value(correct_name, self.whitelist_vars[correct_name])
                    print(f"  [OK] Preserved {correct_name} (whitelisted)")
                    preserved_count += 1
            else:
                self.delete_user_env_value(var_name)
                print(f"  [OK] Removed {var_name}")
                removed_count += 1

        for var_name, var_value in self.whitelist_vars.items():
            if var_name.upper() != "PATH":
                if var_name.upper() not in {v.upper() for v in all_vars.keys()}:
                    self.set_user_env_value(var_name, var_value)
                    print(f"  [OK] Created {var_name} (from whitelist)")
                    preserved_count += 1

        if self.whitelist_paths and (path_was_deleted or "PATH" not in {v.upper() for v in all_vars.keys()}):
            # Create PATH with whitelist entries
            self.set_user_env_value("PATH", ";".join(self.whitelist_paths))
            print(f"  [OK] Created PATH ({len(self.whitelist_paths)} whitelisted entries)")
            preserved_count += 1
        elif self.whitelist_paths and not path_was_deleted:
            # PATH exists but may not have all whitelist entries - ensure they're all there
            current_path = self.get_all_user_env_vars().get("PATH", "")
            current_paths = [p.strip() for p in current_path.split(";") if p.strip()] if current_path else []

            # Merge whitelist paths with current (whitelist first to prioritize)
            merged = self.merge_paths(self.whitelist_paths, current_paths)
            if len(merged) > len(current_paths):
                self.set_user_env_value("PATH", ";".join(merged))
                print(f"  [OK] Updated PATH to include all {len(self.whitelist_paths)} whitelisted entries")

        print(f"[OK] Clean completed - {removed_count} removed, {preserved_count} preserved")
        self.broadcast_environment_change()

    def sync(self):
        print("\n1. Cleaning user environment variables...")
        self.clean()

        print("\n2. Syncing variables to user environment...")
        for var_name, var_value in self.add_vars.items():
            self.set_user_env_value(var_name, var_value)
            print(f"  [OK] Set {var_name}")

        if self.add_paths:
            current_path = self.get_all_user_env_vars().get("PATH", "")
            current_paths = [p.strip() for p in current_path.split(";") if p.strip()] if current_path else []
            merged_paths = self.merge_paths(self.add_paths, current_paths)
            self.set_user_env_value("PATH", ";".join(merged_paths))
            print(f"  [OK] Set PATH ({len(merged_paths)} entries: {len(self.add_paths)} added + {len(current_paths)} existing)")

        print("\n3. Ensuring whitelisted variables exist...")
        for var_name, var_value in self.whitelist_vars.items():
            all_vars = self.get_all_user_env_vars()
            if var_name.upper() not in {v.upper() for v in all_vars.keys()} and var_name.upper() not in {v.upper() for v in self.add_vars.keys()}:
                self.set_user_env_value(var_name, var_value)
                print(f"  [OK] Created {var_name} (from whitelist)")

        total_synced = len(self.add_vars) + (1 if self.add_paths else 0)
        print(f"\n[OK] Sync completed! {total_synced} variables synced")
        self.broadcast_environment_change()
        print("\n[INFO] New processes will pick up the changes immediately")
        print("[INFO] Running processes (like VS Code) need to be restarted")
        return True


def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("  mise_env_sync.exe sync     - Sync variables to user environment")
        print("  mise_env_sync.exe clean    - Remove user environment variables (preserve whitelist)")
        print("")
        print("Configuration via environment variables:")
        print("  MISE_ADD_VARS    - Variables to export (e.g., VAR1=value1;VAR2=value2)")
        print("  MISE_ADD_PATHS   - PATH entries to export (e.g., C:\\path1;C:\\path2)")
        print("  MISE_KEEP_VARS   - Variables to keep (e.g., TEMP=%USERPROFILE%\\Temp)")
        print("  MISE_KEEP_PATHS  - PATH entries to keep (e.g., C:\\Windows\\system32)")
        sys.exit(1)

    command = sys.argv[1].lower()
    syncer = MiseEnvSync()

    if command == "sync":
        syncer.sync()
    elif command == "clean":
        syncer.clean()
    else:
        print(f"Unknown command: {command}")
        print("Use 'sync' or 'clean'")
        sys.exit(1)


if __name__ == "__main__":
    main()
