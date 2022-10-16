function concat(string $x, string $y): string {
	$x = $x + $y;
}

function nulltest(?string $a, ?float $float): ?int {
	$a = "A testing string.";
	return $float + 1;
}
