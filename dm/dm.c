
#include "dm.h"
#include "skinloader.h"

int dm_session() {

}

int dm_run() {
	/*
	 * dm does this:
	 * - loads skin
	 * - cycles starting X server and spawns a session
	 * - when detects problems or exit signal, gives up (optionally
	 *   terminating the server)
	 * - unloads skin
	 */

	return 0;
}
