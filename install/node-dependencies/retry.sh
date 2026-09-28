retry() {
  local n=1 max=3 delay=15
  until "$@"; do
    if [ $n -ge $max ]; then
      echo "Command failed after $n attempts: $*"
      return 1
    fi
    echo "Attempt $n failed, retrying in ${delay}s: $*"
    sleep $delay
    n=$((n + 1))
  done
}
