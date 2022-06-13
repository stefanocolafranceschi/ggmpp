hadd -f plots.root `find /ggmdata/DATA -type f -mtime -1 -size +300k -size -500k | grep .root`

