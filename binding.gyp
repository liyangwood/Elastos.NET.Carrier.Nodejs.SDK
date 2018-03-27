{
  "targets": [
    {
      "target_name": "elca",
      "sources": [
        "src/elca.cc",
        "src/callback.cc",
        "src/datatype.cc",
        "src/friend.cc",
        "src/nodeinfo.cc",
      ],
     "include_dirs": ["./deps/include"],
     "libraries": [
            "-L./deps/lib",
            "-lelacommon",
            "-lelacarrier",
            "-lelasession"
          ],
    }
  ]
}
