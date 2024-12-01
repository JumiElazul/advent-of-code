case File.read("input/day1_input.txt") do
  {:ok, content} ->
    IO.puts("File content:")
    IO.puts(content)

  {:error, reason} ->
    IO.puts("Failed to read file: #{reason}")
end


