/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Common.h
 * @author Alex Leverington <nessence@gmail.com>
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * Ethereum-specific data structures & algorithms.
 */

#pragma once

#include <libdevcore/Address.h>
#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>

namespace dev
{

using Secret = SecureFixedHash<32>;

/// A public key: 64 bytes.
/// @NOTE This is not endian-specific; it's just a bunch of bytes.
using Public = h512;

/// A signature: 65 bytes: r: [0, 32), s: [32, 64), v: 64.
/// @NOTE This is not endian-specific; it's just a bunch of bytes.
using Signature = h520;

struct SignatureStruct
{
	SignatureStruct() = default;
	SignatureStruct(Signature const& _s) { *(h520*)this = _s; }
    SignatureStruct(h256 const& _r, h256 const& _s, byte _v) : r(_r), s(_s), v(_v) {}
    operator Signature() const { return *(h520 const*)this; }

	/// @returns true if r,s,v values are valid, otherwise false
	bool isValid() const noexcept;

    h256 r;
    h256 s;
    byte v = 0;
};

/// Convert a secret key into the public key equivalent.
Public toPublic(Secret const& _secret);

/// Convert a public key to address.
Address toAddress(Public const& _public);

/// Convert a secret key into address of public key equivalent.
/// @returns 0 if it's not a valid secret key.
Address toAddress(Secret const& _secret);

// Convert transaction from and nonce to address.
Address toAddress(Address const& _from, u256 const& _nonce);

/// Recovers Public key from signed message hash.
Public recover(Signature const& _sig, h256 const& _hash);
	
/// Returns siganture of message hash.
Signature sign(Secret const& _k, h256 const& _hash);
	
/// Verify signature.
bool verify(Public const& _k, Signature const& _s, h256 const& _hash);
}
