/**
 * Copyright (c) 2013 MasterWallet developers (see AUTHORS)
 *
 * This file is part of MasterWallet <https://github.com/duzy/masterwallet>.
 *
 * libwallet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __MASTER_WALLET_H___DUZY__
#define __MASTER_WALLET_H___DUZY__ 1
#include "all.h"
#include "addressbook.h"
#include "transaction.h"
#include <list>

namespace mastercoin
{
    constexpr std::uint32_t thread_pool_size = 4;

    enum class currency : std::uint32_t
    {
	BTC = 0, MSC = 1, TMSC = 2,
    };

    constexpr std::array<const char *, 3> symbols
    {
	{ "BTC", "MSC", "TMSC" }
    };
    
    constexpr const char * symbol(currency c) { return symbols[(int) c]; }

    /**
     *  A amount of money of a currency.
     */
    class money
    {
    public:
	money(mastercoin::currency c, std::uint64_t a) : currency(c), amount(a) {}

	const char * get_symbol() const { return symbol(currency); }
	mastercoin::currency get_currency() const { return currency; }
	std::uint64_t get_amount() const { return amount; }

	void set_currency(currency c) { currency = c; }
	void set_amount(std::uint64_t a) { amount = a; }

    private:
	mastercoin::currency currency;
	std::uint64_t amount;
    };

    /**
     *  Represent wallet balance of all currencies.
     */
    struct balance : std::array<std::uint64_t, symbols.size()>
    {
	void set_amount(currency cc, std::uint64_t amount) { (*this)[(int)cc] = amount; }
	void set_amount(const money & m)                   { (*this)[(int)m.get_currency()] = m.get_amount(); }
	void add_amount(currency cc, std::uint64_t amount) { (*this)[(int)cc] += amount; }
	void add_amount(const money & m)                   { (*this)[(int)m.get_currency()] += m.get_amount(); }
	std::uint64_t get_amount(currency cc) const        { return (*this)[(int)cc]; }
    };

    class master_wallet
    {
    public:
	master_wallet();
	
	virtual ~master_wallet();
	
	void start();
	void stop();
	void join();

	std::size_t get_address_count() const { return addresses_.size(); }
	bitcoin::payment_address get_address(int n) const { return addresses_[n].address; }
	balance get_address_balance(int n) const { return addresses_[n].balance; }

	money get_balance(currency cc) const;

	void add_address(const bitcoin::payment_address & address, const money & m);

	void send(const bitcoin::payment_address & addressSource, const bitcoin::payment_address & addressDest, const money & m);

    private:
	struct address_details
	{
	    bitcoin::payment_address address;
	    mastercoin::balance balance;
	};

	void txcast();
	void process_tx(const transaction & tx);

    private:
	bitcoin::threadpool pool_;
	std::vector<address_details> addresses_;
	address_book addbook_;
    };

}//namespace mastercoin

#endif//__MASTER_WALLET_H___DUZY__
