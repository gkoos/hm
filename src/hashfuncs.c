long strHash(const char *s, const int p, const long m) {
    long long hashValue = 0;
    long long pPow = 1;
    while(*s != '\0') {
        hashValue = (hashValue + *s++ * pPow) % m;
        pPow = (pPow * p) % m;
    }
    return hashValue;
}

long strDoubleHash(const char *s, const long m, const int p1, const int p2, const int numAttempts) {
    const long hash1 = strHash(s, p1, m);
    if (numAttempts == 0) {
      return hash1;
    }
    else {
      const long hash2 = strHash(s, p2, m);
      return (hash1 + (numAttempts * (p2 - hash2 % p2))) % m;
    }
}