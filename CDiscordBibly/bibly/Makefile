bibly: bibly.c bibly.h util.h
	cc bibly.c -o bibly -I. -Iconcord/include -Iconcord/core -Iconcord/core/third-party -Iconcord/gencodecs -Iconcord/cog-utils -L. -Lconcord/lib -pthread -ldiscord -lcurl

.PHONY: clean

clean:
	rm bibly
