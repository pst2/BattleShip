name: Update Profile README

on:
  schedule:
    - cron: "0 0 * * *"  # Chạy hàng ngày vào 00:00 UTC
  workflow_dispatch:  # Cho phép chạy thủ công

jobs:
  update-readme:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Update README
        run: |
          echo "### Last updated: $(date)" > README.md

      - name: Commit and push
        run: |
          git config --global user.name "github-actions[bot]"
          git config --global user.email "github-actions[bot]@users.noreply.github.com"
          git add README.md
          git commit -m "Auto update README" || echo "No changes to commit"
          git push
