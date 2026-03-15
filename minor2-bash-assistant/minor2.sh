#!/usr/bin/env bash
# Kiyana Baghbadrani

set -euo pipefail
IFS=$'\n\t'

if [[ "${1:-}" == "--debug" ]]; then
  set -x
fi

LOG_FILE="${HOME}/minor2_actions.log"
declare -a REMINDERS=()
MAX_REMINDERS=5

log() { printf '[%(%F %T)T] %s\n' -1 "$*" >> "$LOG_FILE"; }

on_interrupt() {
  echo "Use the menu to exit safely."
  log "Signal caught; user attempted to interrupt"
}
trap on_interrupt SIGINT SIGTERM

daily_briefing() {
  local user_name
  user_name="$(id -un)"
  echo "Good morning, ${user_name}!"
  echo -n "Date and Time: "; date
  echo "Calendar:"
  cal
  echo "Running Processes: $(ps -e --no-headers | wc -l)"
  log "Displayed daily briefing"
}

organize_files() {
  read -rp "Enter directory to organize: " dir
  if [[ ! -d "$dir" ]]; then
    echo "Not a directory."
    log "Organize failed: '$dir' not dir"
    return
  fi
  echo "Scanning $dir..."
  log "Organize started for $dir"
  total=$(find "$dir" -maxdepth 1 -type f | wc -l | tr -d ' ')
  echo "Total files: $total"
  echo "Text files:"
  find "$dir" -maxdepth 1 -type f -name '*.txt' -print
  echo "Shell scripts:"
  find "$dir" -maxdepth 1 -type f -name '*.sh' -print
  echo "Log files:"
  find "$dir" -maxdepth 1 -type f -name '*.log' -print
  if find "$dir" -maxdepth 1 -type f -name '*.log' | grep -q .; then
    mkdir -p "$dir/logs"
    find "$dir" -maxdepth 1 -type f -name '*.log' -exec mv {} "$dir/logs/" \;
    echo "Log files moved to $dir/logs/"
    log "Moved .log files into $dir/logs/"
  else
    echo "No .log files to move."
    log "No .log files found in $dir"
  fi
}

add_reminder() {
  if (( ${#REMINDERS[@]} >= MAX_REMINDERS )); then
    echo "Reminder list full (max ${MAX_REMINDERS})."
    log "Add reminder failed: full"
    return
  fi
  read -rp "Enter reminder: " r
  REMINDERS+=("$r")
  log "Added reminder: $r"
}

view_reminders() {
  echo "Reminders:"
  for i in "${!REMINDERS[@]}"; do
    echo "$i: ${REMINDERS[$i]}"
  done
  log "Viewed reminders"
}

delete_reminder() {
  read -rp "Enter index to delete: " idx
  if [[ "$idx" =~ ^[0-9]+$ ]] && (( idx < ${#REMINDERS[@]} )); then
    log "Deleted reminder index $idx: ${REMINDERS[$idx]}"
    unset 'REMINDERS[idx]'
    REMINDERS=("${REMINDERS[@]}")
    echo "Deleted."
  else
    echo "Invalid index."
    log "Delete reminder failed: invalid index '$idx'"
  fi
}

health_check() {
  read -rp "Enter your heart rate: " hr
  if ! [[ "$hr" =~ ^[0-9]+$ ]]; then
    echo "Please enter a valid number."
    log "Health check invalid input: $hr"
    return
  fi
  if (( hr < 60 )); then
    echo "Heart rate is Low."
  elif (( hr <= 100 )); then
    echo "Heart rate is Normal."
  else
    echo "Heart rate is High."
  fi
  log "Health check: $hr bpm"
}

main_menu() {
  PS3="Enter choice: "
  select opt in "Organize Files" "Add Reminder" "View Reminders" "Delete Reminder" "Health Check" "Exit"; do
    case $REPLY in
      1) log "Menu: Organize Files"; organize_files ;;
      2) log "Menu: Add Reminder"; add_reminder ;;
      3) log "Menu: View Reminders"; view_reminders ;;
      4) log "Menu: Delete Reminder"; delete_reminder ;;
      5) log "Menu: Health Check"; health_check ;;
      6) log "Menu: Exit"; echo "Goodbye!"; break ;;
      *) echo "Invalid choice."; log "Menu: invalid '$REPLY'";;
    esac
  done
}

daily_briefing
main_menu
