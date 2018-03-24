{
  "targets": [
    {
      "target_name": "carrier",
      "sources": [
        "src/carrier.cc"
      ],
     "include_dirs": ["./deps/include"],
     "libraries": [
            "-L./deps/lib"
          ],
    }
  ]
}
