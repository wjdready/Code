

```sh
# Enable tab completion with menu
Set-PSReadLineKeyHandler -Key Tab -Function MenuComplete
Set-PSReadLineKeyHandler -Key Shift+Tab -Function Complete
Set-PSReadLineOption -CompletionQueryItems 100

# Show history list view instead of inline (ignore errors in non-interactive terminals)
try {
    Set-PSReadLineOption -PredictionSource History -ErrorAction Stop
    Set-PSReadLineOption -PredictionViewStyle ListView -ErrorAction Stop
} catch {
    # Silently ignore if terminal doesn't support predictions
}

# Emacs/Vim-style keybindings
Set-PSReadLineKeyHandler -Chord Ctrl+a -Function BeginningOfLine
Set-PSReadLineKeyHandler -Chord Ctrl+e -Function EndOfLine
Set-PSReadLineKeyHandler -Chord Ctrl+n -Function NextHistory
Set-PSReadLineKeyHandler -Chord Ctrl+p -Function PreviousHistory
Set-PSReadLineKeyHandler -Chord Ctrl+f -Function ForwardChar
Set-PSReadLineKeyHandler -Chord Ctrl+b -Function BackwardChar
Set-PSReadLineKeyHandler -Chord Ctrl+d -Function DeleteChar
Set-PSReadLineKeyHandler -Chord Ctrl+u -Function BackwardKillLine
Set-PSReadLineKeyHandler -Chord Ctrl+w -Function BackwardKillWord

# Word movement
Set-PSReadLineKeyHandler -Chord Alt+f -Function ForwardWord
Set-PSReadLineKeyHandler -Chord Alt+b -Function BackwardWord
Set-PSReadLineKeyHandler -Chord Alt+d -Function KillWord

# Disable Alt+a (prevent SelectAll or other default behavior)
Set-PSReadLineKeyHandler -Chord Alt+a -ScriptBlock {}

# Disable Python venv default prompt modification
$env:VIRTUAL_ENV_DISABLE_PROMPT = 1

function ma {
    mise activate pwsh | Out-String | Invoke-Expression
}

# Custom prompt with newline (like bash)
function prompt {
    # Add blank line before prompt (except first prompt)
    if ($global:PromptCount -gt 0) {
        Write-Host ""
    }
    $global:PromptCount++

    $user = $env:USERNAME
    $host_name = $env:COMPUTERNAME
    $location = Get-Location

    Write-Host "$user@$host_name " -NoNewline -ForegroundColor Green

    # Show virtual environment if active
    if ($env:VIRTUAL_ENV) {
        $venv_name = Split-Path $env:VIRTUAL_ENV -Leaf
        Write-Host "($venv_name) " -NoNewline -ForegroundColor Yellow
    }

    Write-Host "$location" -ForegroundColor Blue
    return "PS> "
}

# Initialize prompt counter
$global:PromptCount = 0
```
