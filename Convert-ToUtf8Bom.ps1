param (
    [string]$RootPath = "."
)

# 변환할 텍스트 파일 확장자
$TargetExtensions = @(
    ".h",
    ".hpp",
    ".c",
    ".cpp",
    ".cc",
    ".cxx",
    ".inl",
    ".rc",
    ".rc2",
    ".def",
    ".idl",
    ".txt",
    ".md",
    ".cmake",
    ".props",
    ".targets",
    ".vcxproj",
    ".filters",
    ".sln"
)

# 변환하지 않을 디렉터리
$ExcludedDirectories = @(
    ".git",
    ".vs",
    "Debug",
    "Release",
    "x64",
    "x86",
    "bin",
    "obj",
    "packages",
    "build",
    "out"
)

# CP949 인코딩 사용을 위해 코드 페이지 공급자 등록
[System.Text.Encoding]::RegisterProvider(
    [System.Text.CodePagesEncodingProvider]::Instance
)

$cp949 = [System.Text.Encoding]::GetEncoding(
    949,
    [System.Text.EncoderExceptionFallback]::new(),
    [System.Text.DecoderExceptionFallback]::new()
)

# 잘못된 UTF-8 바이트가 있으면 예외를 발생시키는 UTF-8
$utf8Strict = [System.Text.UTF8Encoding]::new(
    $false, # BOM 없음
    $true   # invalid byte 발생 시 예외
)

# 출력용 UTF-8 BOM
$utf8Bom = [System.Text.UTF8Encoding]::new(
    $true,  # BOM 있음
    $true
)

$root = (Resolve-Path $RootPath).Path

$convertedCount = 0
$skippedCount = 0
$errorCount = 0

$files = Get-ChildItem `
    -Path $root `
    -File `
    -Recurse `
    -Force |
Where-Object {
    $file = $_

    $isExcluded = $false

    foreach ($directory in $ExcludedDirectories) {
        $directoryPattern = [regex]::Escape(
            [System.IO.Path]::DirectorySeparatorChar + $directory +
            [System.IO.Path]::DirectorySeparatorChar
        )

        if ($file.FullName -match $directoryPattern) {
            $isExcluded = $true
            break
        }
    }

    if ($isExcluded) {
        return $false
    }

    return $TargetExtensions -contains $file.Extension.ToLowerInvariant()
}

foreach ($file in $files) {
    try {
        $bytes = [System.IO.File]::ReadAllBytes($file.FullName)

        if ($bytes.Length -eq 0) {
            Write-Host "[SKIP] Empty file: $($file.FullName)"
            $skippedCount++
            continue
        }

        $detectedEncoding = ""

        # UTF-8 BOM 확인: EF BB BF
        if (
            $bytes.Length -ge 3 -and
            $bytes[0] -eq 0xEF -and
            $bytes[1] -eq 0xBB -and
            $bytes[2] -eq 0xBF
        ) {
            $text = $utf8Strict.GetString(
                $bytes,
                3,
                $bytes.Length - 3
            )

            $detectedEncoding = "UTF-8 BOM"
        }
        else {
            try {
                # BOM은 없지만 정상적인 UTF-8인지 검사
                $text = $utf8Strict.GetString($bytes)
                $detectedEncoding = "UTF-8 no BOM"
            }
            catch [System.Text.DecoderFallbackException] {
                # UTF-8이 아니면 CP949로 간주
                $text = $cp949.GetString($bytes)
                $detectedEncoding = "CP949"
            }
        }

        # UTF-8 BOM으로 저장
        [System.IO.File]::WriteAllText(
            $file.FullName,
            $text,
            $utf8Bom
        )

        Write-Host "[OK] $detectedEncoding -> UTF-8 BOM: $($file.FullName)"
        $convertedCount++
    }
    catch {
        Write-Warning "[ERROR] $($file.FullName)"
        Write-Warning $_.Exception.Message
        $errorCount++
    }
}

Write-Host ""
Write-Host "======================================"
Write-Host "Converted : $convertedCount"
Write-Host "Skipped   : $skippedCount"
Write-Host "Errors    : $errorCount"
Write-Host "======================================"